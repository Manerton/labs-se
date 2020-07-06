#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "p_semiconductor.h"
#include "n_semiconductor.h"
#include "diod.h"
#include "transistor.h"
#include "halfbitsummator.h"
#include "twobitsummator.h"

class PictureBox : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PictureBox(QWidget *parent = nullptr);
    ~PictureBox();

private:
    QGraphicsScene *scene;          // Объявляем сцену для отрисовки

    // для визуализации p-полупроводника
    P_semiconductor *p_semi = nullptr;
    // для визуализации n-полупроводника
    N_semiconductor *n_semi = nullptr;
    // для визуализации p-n перехода
    Diod *perehod = nullptr;
    // для визуализации транзистора n-p-n
    Transistor *trans = nullptr;
    // для визуализации полубитного сумматора
    HalfBitSummator *halfbit = nullptr;
    // для визуализации двубитного сумматора
    TwoBitSummator *twobit = nullptr;
    // позволяем увеличивать размеры окна с масштабированием содержимого
    void resizeEvent(QResizeEvent *event);
    // остановить все на паузу и скрыть все визуализации со сцены
    void pauseAll();
public:
    void show_p_semi();
    void show_n_semi();
    void show_diod();
    void show_transistor();
    void show_halfbitsum();
    void show_twobitsum();
    void callHalfBitSum(bool x, bool y);
    void callTwoBitSum(bool a, bool b, bool c, bool d);
    // рисуем знак + или -, или любой другой символ 'c' по коордианте coords и помещаем элемент в указанную группу group
    static QGraphicsTextItem *drawSigns(const std::string &c, const QPointF &coords, QGraphicsItemGroup *group, int FontSize = 28);
    // переадресовываем для p-n перехода, что необходимо изменить полярность
    void changePolarity();
private slots:
    // переадресовываем для p_semi, что необходимо сбросить анимацию в стартовую позицию
    // это вызывается, когда анимация закончилась в демонстрации p_semi
    // и необходимо её запустить заново (для цикличности анимации)
    void reset_Animations_for_p_semi();
};
#endif // PICTUREBOX_H

