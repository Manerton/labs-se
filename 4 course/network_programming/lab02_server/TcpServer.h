#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>

class QTcpServer;
class QTcpSocket;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr, uint16_t _port = 11000);

private slots:

    /// Новое соединение.
    void newConnection();

    /// Сокет готов читать.
    void readyRead(QTcpSocket &socket);

    /// Клиент отсоединился от сервера.
    void clientDisconnected(QTcpSocket &socket);

private:

    /// TCP сервер.
    QTcpServer *server;

    /// Порт сервера.
    const uint16_t port;
};

#endif // TCPSERVER_H
