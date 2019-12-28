#include <QFrame>
#include <QPainter>
#include <QPixmap>
#include "kmp.h"
#ifndef PICTUREBOX_H
#define PICTUREBOX_H
class PictureBox : public QFrame
{
    Q_OBJECT
    enum class DemoMode
    {
        drawPrefFunc = 0,
        drawPrefFunc_result = 1,
        drawKMP = 2
    };

    struct drawSettings                             // -- настройки для демонстрации  -- //
    {
        QFont font;                                 // -- шрифт для символов -- //
        QFont font_for_num;                         // -- шрифт для чисел -- //
        int cell_size;                              // -- размер ячейки -- //
        QPoint p;                                   // -- координаты для отрисовки -- //
    };

    QPixmap m_Pixmap;                               // -- буфер для отрисовки -- //
    drawSettings settings;                          // -- настройки -- //

    QString str;                                    // -- строка -- //
    QString substr;                                 // -- подстрока-образец -- //

    size_t i_step = 1;                              // -- номер шага -- //
    size_t N_step = 0;                                  // -- количество шагов -- //
    std::vector<pref_func_step_info> pref_steps;    // -- шаги для префиксной функции -- //
    std::vector<kmp_step_info> kmp_steps;           // -- шаги КМП -- //
    std::vector<int> Pi;                            // -- префиксная функция -- //
    DemoMode current_mode;                          // -- текущий режим демонстрации -- //
    friend class Demo;
public:
    explicit PictureBox(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent*);          // функция, которая рисует что либо

private:
    drawSettings prepare_drawSettings(int length); // -- подготовка настроек -- //
    void drawPrefFunc(QPainter &painter);
    void drawPrefFunc_result(QPainter &painter);
    void drawKMP(QPainter &painter, int anim_x);
    void drawPiArray(QPainter &painter);
    void do_step(bool next);                                // -- отрисовать следующий шаг -- //
    void animation_podstroka(QPainter &painter, int x, int y);
    void do_next_step_kmp(QPainter &painter);
    void do_prev_step_kmp(QPainter &painter);
};
#endif // PICTUREBOX_H
