#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QThreadPool>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    /// Тип потенциала клетки.
    using Potential = uint8_t;
    /// Тип состояния клетки.
    using State = uint8_t;
    /// Тип строки таблицы с описанием состояний клеток.
    using TableRow = std::vector<State>;
    /// Тип таблицы с описанием состояний клеток.
    using LifeTable = std::vector<TableRow>;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

    /// Началось ли отображение состояния клеточной модели "Игра Жизнь"?
    bool isStarted = false;

    /// Таймер для отображения
    std::unique_ptr<QTimer> renderTimer = nullptr;

    /// Последняя таблица с описанием состояний клеток
    LifeTable latestTable;

    /// Время одного шага (в мс)
    static constexpr int32_t oneStepTime = 200;

    /// Размер виджета.
    static constexpr int32_t widgetSize = 800;

    /// Размер одной клетки.
    static constexpr int32_t cellSize = 20;

    /// Количество клеток на экране (по горизонтали или вертикали).
    static constexpr int32_t cellCount = widgetSize / cellSize;

    /// Ширина кнопки.
    static constexpr int32_t buttonW = 100;

    /// Высота кнопки.
    static constexpr int32_t buttonH = 30;

    /// Правила изменения состояния клетки.
    State lifeRules(const Potential p, const State state) const;
    /// Потенциал клетки вычисляется по восьми ближайшим соседям клетки.
    Potential calcPotential(const LifeTable &table, int32_t i, int32_t j) const;
    /// Получить начальную таблицу из начального выделения.
    LifeTable getInitTable() const;
    /// Последовательный алгоритм расчета состояния каждой клетки.
    LifeTable seqCalcNewTable(const LifeTable &oldTable) const;
    /// Паралелльный алгоритм расчета состояния каждой клетки по строкам.
    LifeTable parCalcNewTableByRow(const LifeTable &oldTable) const;
    /// Паралелльный алгоритм расчета состояния каждой клетки по столбцам.
    LifeTable parCalcNewTableByColumn(const LifeTable &oldTable) const;
    /// Отрисовать таблицу на экране.
    void renderLifeTable(const LifeTable &table) const;
    /// Один шаг отображения.
    void doOneStep();
    /// Начать отображение состояния клеточной модели "Игра Жизнь".
    void start();
    /// Прекратить отображение состояния клеточной модели
    void stop();
};
#endif // WIDGET_H
