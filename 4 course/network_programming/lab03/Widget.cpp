#include "Widget.h"
#include "./ui_Widget.h"

#include <limits>
#include <thread>

#include <QtNetwork/QHostAddress>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->getOpenPorts_DoStep();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::checkConnected(QTcpSocket *socket)
{
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        qDebug() << socket->peerPort();
    }

    socket->abort();
}


void Widget::disconnected(QTcpSocket *socket)
{
    --countPendingConnections;
}

void Widget::stateChanged(QTcpSocket *socket)
{
    // Если состояние "не подключен" и "не подключается", то
    // уменьшаем счётчик обрабатываемых подключений и помечаем сокет на удаление.
    if (socket->state() != QTcpSocket::ConnectedState
            && socket->state() != QTcpSocket::ConnectingState)
    {
        --countPendingConnections;
        socket->deleteLater();
    }

    // Выполняем новый шаг алгоритма, если больше никаких подключений не обрабатывается.
    if (countPendingConnections == 0)
    {
        getOpenPorts_DoStep();
    }
}

void Widget::getOpenPorts_DoStep()
{
    if (step >= countSteps) return;

    // Диапазон сканируемых портов
    const uint16_t start_port = 1;
    const uint16_t end_port = std::numeric_limits<uint16_t>::max();

    // Каких-то проверок диапазона и размера одного шага - нет.
    // То есть необходимо подбирать такое количество шагов, чтобы диапазон делился нацело.
    // В качестве примера: (65535 - 1) / 14 = 65534 / 14 = 4681.
    const uint16_t steps_size = (end_port - start_port) / countSteps;

    const uint16_t start = step * steps_size + 1;
    const uint16_t end = start + steps_size;

    for (uint16_t j = start; j < end; ++j)
    {
        checkPort(j);
    }

    ++step;
}

void Widget::checkPort(uint16_t port)
{
    // Увеличиваем счётчик количества соединений в обработке.
    ++countPendingConnections;

    // Создаем соединение для проверки порта.
    auto socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(QHostAddress::LocalHost), port);

    // Ждём 10 миллисекунд и проверяем - удалось ли соединение.
    QTimer timer(this);
    const int time = 10;
    timer.singleShot(time, this, [this, socket]{ this->checkConnected(socket); });

    // Также привязываем обработку событий отключения соединения и изменение состояния соединения.
    connect(socket, &QTcpSocket::disconnected, this, [this, socket]{ this->disconnected(socket); } );
    connect(socket, &QTcpSocket::stateChanged, this, [this, socket]{ this->stateChanged(socket); } );
}

