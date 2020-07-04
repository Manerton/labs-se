#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "p_semiconductor.h"
#include "n_semiconductor.h"
#include "diod.h"
#include "halfbitsummator.h"
//#include "mainwindow.h"

class PictureBox : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PictureBox(QWidget *parent = nullptr);
    ~PictureBox();

private:
    QGraphicsScene *scene;          // Объявляем сцену для отрисовки

    // для визуализации p-полупроводника
    p_semiconductor *p_semi = nullptr;
    // для визуализации n-полупроводника
    n_semiconductor *n_semi = nullptr;
    // для визуализации p-n перехода
    diod *perehod = nullptr;
    // для визуализации полубитного сумматора
    HalfBitSummator *halfbit = nullptr;
    // позволяем увеличивать размеры окна с масштабированием содержимого
    void resizeEvent(QResizeEvent *event);
    void deleteItemsFromGroup(QGraphicsItemGroup *group);
    void pauseAll();
public:
    void show_p_semi();
    void show_n_semi();
    void show_diod();
    void show_halfbitsum();
    void callHalfBitSum(bool x, bool y);
    // рисуем знак + или -
    static QGraphicsTextItem *drawSign(const char c, const QPointF &coords, QGraphicsItemGroup *group);
    void changePolarity();
public slots:
    void pauseAnim();
    void unpauseAnim();
private slots:
    void resetAnimations();
};
#endif // PICTUREBOX_H

