#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QList>

#include <QtNetwork/QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

/// Сканер открытых TCP портов локального адреса.
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    /// Подключились к порту.
    void checkConnected(QTcpSocket *socket);

    /// Изменилось состояние соединения.
    void stateChanged(QTcpSocket *socket);

    void on_pushButton_exec_clicked();
    void on_pushButton_stop_clicked();
    void on_spinBox_end_editingFinished();
    void on_spinBox_start_editingFinished();

private:
    /// Один шаг алгоритма сканера портов.
    void getOpenPorts_DoStep();

    /// Включить кнопку "Старт" и выключить кнопку "Стоп".
    void allowStartButton();

    /// Включить кнопку "Стоп" и выключить кнопку "Старт".
    void allowStopButton();

    /// Обработка отключенного соединения (уменьшаем счётчик обрабатываемых подключений и помечаем сокет на удаление).
    void handleDisconnectedSocket(QTcpSocket *socket);

    /// Проверка порта "port".
    void checkPort(uint16_t j);

    /// Указатель на интерфейс.
    Ui::Widget *ui;

    /// В работе ли алгоритм?
    bool isWorking = false;

    /// Количество открытых соединений.
    uint16_t countPendingConnections = 0;

    /// Начальный порт для сканера.
    uint16_t startPort = 1;

    /// Конечный порт для сканера.
    uint16_t endPort = 65535;

    /// Количество шагов поиска портов
    uint16_t countSteps = 255;

    /// Количество соединений создаваемых за один шаг алгоритма.
    /// Каких-то особых проверок диапазона и размера одного шага - нет.
    /// То есть необходимо подбирать такое количество шагов, чтобы диапазон делился нацело.
    /// В качестве примера: (65535 - 1 + 1) / 15 = 65535 / 15 = 4369 открывается соединений за один шаг алгоритма.
    /// Для диапазона [1; 65535].
    uint16_t stepSize = (endPort - startPort + 1) / countSteps;

    /// Шаг алгоритма поиска портов
    uint16_t step = 0;

    /// Время ожидания подключения к порту.
    const uint16_t timeout = 10;
};
#endif // WIDGET_H
