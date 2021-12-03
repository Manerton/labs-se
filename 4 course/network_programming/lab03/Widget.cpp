#include "Widget.h"
#include "./ui_Widget.h"

#include <limits>
#include <thread>

#include <QtNetwork/QHostAddress>
#include <QTimer>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->allowStartButton();
}

Widget::~Widget()
{
    delete ui;
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

    // В случае, если удалось соединится, то добавляем порт в список.
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        ui->listWidget_ports->addItem(QString::number(socket->peerPort()));
    }

    // Также привязываемся к событию изменения состояния соединения.
    // Поскольку мы все ещё не отключились от этого порта (не "UnconnectedState"),
    // а значит нужно обработать событие когда состояние изменится на "UnconnectedState".
    connect(socket, &QTcpSocket::stateChanged, this, [this, socket]{ this->stateChanged(socket); } );

    // Принудительно отключаем, поскольку "timeout" прошел
    // и за это время мы уже должны были подключиться.
    socket->abort();
}

void Widget::stateChanged(QTcpSocket *socket)
{
    if (socket->state() == QTcpSocket::UnconnectedState)
    {
        this->handleDisconnectedSocket(socket);
    }
}

void Widget::getOpenPorts_DoStep()
{
    if (this->step >= this->countSteps)
    {
        qDebug() << "end";
        this->isWorking = false;
        this->allowStartButton();
        return;
    }

    const uint16_t start = step * stepSize + this->startPort;
    const uint16_t end = start + stepSize - 1;

    QString currentStepText = QString::number(this->step + 1) + " (" + QString::number(start) + ":" + QString::number(end) + ")";
    ui->label_currentStepValue->setText(currentStepText);

    for (uint32_t j = start; j <= end; ++j)
    {
        checkPort(uint16_t(j));
    }

    ++step;
}

void Widget::allowStartButton()
{
    ui->pushButton_exec->setDisabled(false);
    ui->pushButton_stop->setDisabled(true);
}

void Widget::allowStopButton()
{
    ui->pushButton_exec->setDisabled(true);
    ui->pushButton_stop->setDisabled(false);
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
    if (countPendingConnections == 0 && isWorking)
    {
        getOpenPorts_DoStep();
    }
    // Больше никаких подключений не обрабатывается, но нажали на "Стоп".
    else if (countPendingConnections == 0 && !isWorking)
    {
        this->allowStartButton();
    }
}

void Widget::checkPort(uint16_t port)
{
    // Увеличиваем счётчик количества соединений в обработке.
    ++countPendingConnections;

    // Создаем соединение для проверки порта.
    auto socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(QHostAddress::LocalHost), port);

    // Ждём 'timeout' миллисекунд и проверяем - удалось ли соединение.
    QTimer::singleShot(timeout, this, [this, socket]{ this->checkConnected(socket); });
}

void Widget::on_pushButton_exec_clicked()
{
    this->startPort = uint16_t(ui->spinBox_start->value());
    this->endPort =  uint16_t(ui->spinBox_end->value());
    this->countSteps = uint16_t(ui->spinBox_steps->value());
    const uint16_t diff = (endPort - startPort + 1);

    // Если нацело делится
    if (diff % countSteps == 0)
    {
        this->stepSize = diff / countSteps;

        if (!this->isWorking)
        {
            this->step = 0;
            ui->listWidget_ports->clear();
            this->isWorking = true;
            this->getOpenPorts_DoStep();
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
