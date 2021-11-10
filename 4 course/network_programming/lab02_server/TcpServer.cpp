#include "TcpServer.h"

#include <QTcpServer>
#include <QTcpSocket>

using namespace std;

TcpServer::TcpServer(QObject *parent, uint16_t _port)
    : QObject(parent)
    , server(new QTcpServer(this))
    , port(_port)
{
    connect(server, &QTcpServer::newConnection, this, &TcpServer::newConnection);

    if (server->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server is started on" << port << "port";
    }
    else
    {
        qDebug() << "Error: server is not started";
    }
}

void TcpServer::newConnection()
{
    const auto socket = server->nextPendingConnection();

    qDebug() << "Client" << socket->peerAddress() << ":" << socket->peerPort() <<  "is connected";

    socket->write("Привет! Hello from TCP server!");

    connect(socket, &QTcpSocket::readyRead, this, [&, socket]() { return readyRead(*socket); });
    connect(socket, &QTcpSocket::disconnected, this, [&, socket]() { return clientDisconnected(*socket); });
}

void TcpServer::readyRead(QTcpSocket &socket)
{
    QByteArray allMsg;
    while (socket.bytesAvailable() > 0)
    {
        auto msg = socket.readAll();
        qDebug() << "Readed: " << msg.length() << " bytes.";
        allMsg.append(msg);
    }
    qDebug() << "Message from client: " << QString::fromUtf8(allMsg);
    socket.write("I've got your message: " + allMsg);
}

void TcpServer::clientDisconnected(QTcpSocket &socket)
{
    qDebug() << "Client is disconnected.";
    socket.close();
}
