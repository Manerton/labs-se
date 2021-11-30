#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QList>
#include <atomic>

#include <QtNetwork/QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    struct PortInfo
    {
        uint16_t portNumber;    ///< Номер порта
        QString localAddress;   ///< Локальный адрес
        QString remoteAddress;  ///< Удаленный адрес
        QString state;          ///< Состояние подключения
    };

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    /// Подключились к порту.
    void checkConnected(QTcpSocket *socket);

    /// Отключились от порта.
    void disconnected(QTcpSocket *socket);

    /// Изменилось состояние соединения.
    void stateChanged(QTcpSocket *socket);

private:
    /// Один шаг алгоритма сканера портов.
    void getOpenPorts_DoStep();

    /// Проверка порта "port".
    void checkPort(uint16_t j);

    /// Указатель на интерфейс.
    Ui::Widget *ui;

    /// Количество открытых соединений.
    std::atomic<uint16_t> countPendingConnections = 0;

    /// Количество шагов поиска портов
    uint16_t countSteps = 14;

    /// Шаг алгоритма поиска портов
    uint16_t step = 0;
};
#endif // WIDGET_H
