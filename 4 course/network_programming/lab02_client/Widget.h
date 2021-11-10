#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QHostAddress>

class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    /// Отправляем сообщение на сервер.
    void on_pushButton_request_clicked();

    /// Подключаемся к серверу.
    void on_pushButton_connect_clicked();

    /// Пришло сообщение от сервера.
    void readyRead();

    /// Подключились к серверу.
    void clientConnected();

    /// Отключились от сервера.
    void clientDisconnected();

private:
    Ui::Widget *ui;

    /// Клиентский сокет.
    QTcpSocket *socket;

    /// IP адрес сервера.
    QHostAddress ip = QHostAddress("127.0.0.1");

    /// Порт сервера.
    uint16_t port = 11000;
};
#endif // WIDGET_H
