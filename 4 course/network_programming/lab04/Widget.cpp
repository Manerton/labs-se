#include "Widget.h"
#include "./ui_Widget.h"

#include <limits>
#include <thread>

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include <QTimer>
#include <QMessageBox>
#include <QProcess>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Получаем Ip и маску.
    auto localAddress = this->getLocalIpAndNetmask();
    ui->label_localIp->setText(ui->label_localIp->text() + localAddress.ip().toString());
    ui->label_netmask->setText(ui->label_netmask->text() + localAddress.netmask().toString());

    // Устанавливаем диапазон Ip для поиска.
    this->initIpRange(localAddress);

    this->allowStartButton();
}

Widget::~Widget()
{
    delete ui;
}

QNetworkAddressEntry Widget::getLocalIpAndNetmask()
{
    auto allInterfaces = QNetworkInterface::allInterfaces();

    for (auto &interface: allInterfaces)
    {
        if (!interface.flags().testFlag(interface.IsLoopBack))
        {
            for (auto &address : interface.addressEntries())
            {
                if (address.ip().protocol() == address.ip().IPv4Protocol)
                {
                    return address;
                }
            }
        }
    }

    return QNetworkAddressEntry();
}

void Widget::initIpRange(const QNetworkAddressEntry &localAddress)
{
    // Берем локальный IP, и ставим стартовым такой, чтобы последний октет был равен 1.
    this->startIp = QHostAddress(localAddress.ip().toIPv4Address() - localAddress.ip().toString().split(".").last().toUInt() + 1);

    // А конечный такой, чтобы последний октет был равен 254.
    const uint8_t to254 = 253;
    this->endIp = QHostAddress(startIp.toIPv4Address() + to254);

    ui->lineEdit_startIp->setText(this->startIp.toString());
    ui->lineEdit_endIp->setText(this->endIp.toString());
}

bool Widget::checkIp(const QHostAddress &ip) const
{
    QString nParam = "-n";
    QString pingCount = "1";
    QString wParam = "-w";

    auto code = QProcess::execute("ping", QStringList({ip.toString(), nParam, pingCount, wParam, QString::number(this->timeout)}));

    return (code == 0);
}

void Widget::findNextActiveIp()
{
    const auto nextAfterEndIp = QHostAddress(this->endIp.toIPv4Address() + 1);

    // Ищем следующий активный Ip (с помощью ping).
    bool activeIp = false;
    do
    {
        this->currentIp = QHostAddress(this->currentIp.toIPv4Address() + 1);
        activeIp = this->checkIp(this->currentIp);

        ui->label_currentIpValue->setText(this->currentIp.toString());
    }
    while (!activeIp && this->currentIp != nextAfterEndIp && this->isWorking);
}

void Widget::execNewSearchingForNextIp(const QHostAddress &ip, const QHostAddress &endIp)
{
    // Повторяем процедуру для следующего ip.
    this->currentStep = 0;
    qDebug() << "Trying work for " << ip << "...";

    const auto nextAfterEndIp = QHostAddress(this->endIp.toIPv4Address() + 1);

    // Если перебрали все Ip, то завершаем поиск.
    if (ip == nextAfterEndIp)
    {
        qDebug() << "End of all work";
        this->isWorking = false;
        this->allowStartButton();
        return;
    }

    // Иначе следующий шаг алгоритма.
    StepInfo info (this->currentStep, this->countSteps, this->stepSize,
                   this->startPort, this->currentIp
                   );
    this->currentStep = getOpenPorts_DoStep(info);
}

void Widget::checkConnected(QTcpSocket *socket)
{

    // Иногда в этой функции сокет уже в состоянии "отключён",
    // обрабатываем этот случай.
    if (socket->state() == QTcpSocket::UnconnectedState)
    {
        handleDisconnectedSocket(socket);
        return;
    }

    if (socket->state() == QTcpSocket::ConnectedState)
    {
        auto rowIndex = ui->tableWidget_ports->rowCount();
        ui->tableWidget_ports->insertRow(rowIndex);
        ui->tableWidget_ports->setItem(rowIndex, 0, new QTableWidgetItem(socket->peerAddress().toString()));
        ui->tableWidget_ports->setItem(rowIndex, 1, new QTableWidgetItem(QString::number(socket->peerPort())));
    }

    socket->abort();
}

void Widget::stateChanged(QTcpSocket *socket)
{
    if (socket->state() == QTcpSocket::UnconnectedState)
    {
        this->handleDisconnectedSocket(socket);
    }
}

uint16_t Widget::getOpenPorts_DoStep(const StepInfo &info)
{
    const uint16_t start = info.currentStep * info.stepSize + info.startPort;
    const uint16_t end = start + info.stepSize - 1;

    QString currentStepText = QString::number(info.currentStep + 1) + " (" + QString::number(start) + ":" + QString::number(end) + ")";
    ui->label_currentStepValue->setText(currentStepText);

    for (uint32_t j = start; j <= end; ++j)
    {
        execCheckingPort(uint16_t(j), info.currentIp);
    }

    return info.currentStep + 1;
}

void Widget::allowStartButton()
{
    ui->pushButton_exec->setDisabled(false);
    ui->pushButton_stop->setDisabled(true);
    QApplication::restoreOverrideCursor();
}

void Widget::allowStopButton()
{
    ui->pushButton_exec->setDisabled(true);
    ui->pushButton_stop->setDisabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void Widget::handleDisconnectedSocket(QTcpSocket *socket)
{
    if (countPendingConnections > 0)
    {
        --countPendingConnections;
    }
    socket->deleteLater();

    // Выполняем новый шаг алгоритма, если больше никаких подключений не обрабатывается.
    // И если не нажали на "Стоп".
    if (this->countPendingConnections == 0 && this->isWorking)
    {
        // TODO: вывод текущего ip в интерфейсе

        // Если алгоритм закончил свою работу.
        if (this->currentStep >= this->countSteps)
        {
            qDebug() << "End work for " << this->currentIp;
            auto thread = QThread::create(&Widget::findNextActiveIp, this);
            connect(thread, &QThread::finished, this, [&](){ this->execNewSearchingForNextIp(this->currentIp, this->endIp); });
            thread->start();
        }
        else
        {
            // Если не закончил, то приступаем к следующему шагу алгоритма.
            StepInfo info (this->currentStep, this->countSteps, this->stepSize,
                           this->startPort, this->currentIp
                           );
            this->currentStep = getOpenPorts_DoStep(info);
        }

    }
    // Больше никаких подключений не обрабатывается, но нажали на "Стоп".
    else if (countPendingConnections == 0 && !isWorking)
    {
        this->allowStartButton();
    }
}

void Widget::execCheckingPort(uint16_t port, const QHostAddress &ip)
{
    // Увеличиваем счётчик количества соединений в обработке.
    ++countPendingConnections;

    // Создаем соединение для проверки порта.
    auto socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);

    // Ждём 'timeout' миллисекунд и проверяем - удалось ли соединение.
    QTimer::singleShot(timeout, this, [this, socket]{ this->checkConnected(socket); });

    // Также привязываем обработку событий отключения соединения и изменение состояния соединения.
    connect(socket, &QTcpSocket::stateChanged, this, [this, socket]{ this->stateChanged(socket); } );
}

void Widget::on_pushButton_exec_clicked()
{
    this->startPort = uint16_t(ui->spinBox_start->value());
    this->endPort =  uint16_t(ui->spinBox_end->value());
    this->countSteps = uint16_t(ui->spinBox_steps->value());
    this->startIp = QHostAddress(ui->lineEdit_startIp->text());
    this->endIp = QHostAddress(ui->lineEdit_endIp->text());

    const uint16_t diff = (endPort - startPort + 1);

    // Если нацело делится
    if (diff % countSteps == 0)
    {
        this->stepSize = diff / countSteps;

        if (!this->isWorking)
        {
            this->isWorking = true;
            ui->tableWidget_ports->setRowCount(0);

            this->currentStep = 0;

            // За начальный берём на 1 меньше, так как метод 'findNextActiveIp'
            // начинает искать активный IP-адрес сразу следующий за this->currentIp
            // (т.е инкриментирует IP перед проверкой)
            this->currentIp = QHostAddress(this->startIp.toIPv4Address() - 1);
            ui->label_currentIpValue->setText(this->currentIp.toString());

            auto thread = QThread::create(&Widget::findNextActiveIp, this);
            connect(thread, &QThread::finished, this, [&](){ this->execNewSearchingForNextIp(this->currentIp, this->endIp); });
            thread->start();

            this->allowStopButton();
        }
    }
    else
    {
        QMessageBox::warning(this, "Неправильные параметры",
                             "Указанный диапазон не делится на количество шагов. "
                             "Измените параметры и попробуйте снова.");
    }
}


void Widget::on_pushButton_stop_clicked()
{
    this->isWorking = false;
}


void Widget::on_spinBox_start_editingFinished()
{
    int start = ui->spinBox_start->value();
    int end = ui->spinBox_end->value();
    if (start >= end)
    {
        ui->spinBox_start->setValue(end-1);
    }
}

void Widget::on_spinBox_end_editingFinished()
{
    int start = ui->spinBox_start->value();
    int end = ui->spinBox_end->value();
    if (end <= start)
    {
        ui->spinBox_end->setValue(start+1);
    }
}
