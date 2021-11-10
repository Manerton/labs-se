#include "Widget.h"
#include "./ui_Widget.h"

#include <QTcpSocket>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // В целях простоты не делаются специальные проверки:
    // на неправильный адрес сервера
    // на ошибки, возникающие при подключении к серверу
    // на ошибки возникающие при получении/отправки сообщения.

    // Привязываемся к событию, когда сервер присылает нам сообщение.
    connect(socket, &QTcpSocket::readyRead, this, &Widget::readyRead);
    // Привязываемся к событию, когда клиент успешно подключается к серверу.
    connect(socket, &QTcpSocket::connected, this, &Widget::clientConnected);
    // Привязываемся к событию, когда клиент отключается от сервера.
    connect(socket, &QTcpSocket::disconnected, this, &Widget::clientDisconnected);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_request_clicked()
{
    socket->write(ui->lineEdit_request->text().toUtf8());
}

void Widget::on_pushButton_connect_clicked()
{
    if (socket->state() == QTcpSocket::SocketState::ConnectedState)
    {
        socket->disconnectFromHost();
    }

    ip = QHostAddress(ui->lineEdit_ip->text());
    port = uint16_t(ui->spinBox_port->value());

    if (socket->state() != QTcpSocket::SocketState::ConnectedState)
    {
        socket->connectToHost(ip, port);
    }
}

void Widget::readyRead()
{
    QByteArray allMsg;
    while (socket->bytesAvailable() > 0)
    {
        auto msg = socket->readAll();
        qDebug() << "Readed: " << msg.length() << " bytes.";
        allMsg.append(msg);
    }
    ui->plainTextEdit_response->appendPlainText(allMsg);
}

void Widget::clientDisconnected()
{
    ui->label_statusText->setText("не подключено");
}

void Widget::clientConnected()
{
    ui->label_statusText->setText("подключено");
}

