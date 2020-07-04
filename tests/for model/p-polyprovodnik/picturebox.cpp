#include "picturebox.h"
#include "QDebug"
#include <QResizeEvent>
#include <vector>
#include <numeric>
#include <cmath>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "particle.h"
#include "mainwindow.h"

using namespace std;

PictureBox::PictureBox(QWidget *parent) : QGraphicsView(parent), p_semi(new p_semiconductor()), n_semi(new n_semiconductor()), perehod(new diod()),
    halfbit(new HalfBitSummator())

{
    // настроим отображение виджета и содержимого
    // отключим скролл бары
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // сглаживание
    setRenderHint(QPainter::Antialiasing);
    setAlignment(Qt::AlignCenter);
    // инициализируем сцену
    scene = new QGraphicsScene();
    setScene(scene);
    scene->addItem(p_semi->group);
    scene->addItem(n_semi->group);
    scene->addItem(perehod->group);
    scene->addItem(halfbit->group);
    n_semi->group->hide();
    perehod->group->hide();
    setViewportUpdateMode(FullViewportUpdate);
    //scene->setSceneRect(0,-200,840,500);
    connect(p_semi->animation_group, &QSequentialAnimationGroup::currentLoopChanged, this, &PictureBox::resetAnimations);
}

PictureBox::~PictureBox()
{
    delete scene;
}

void PictureBox::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::fitInView(-70,0,840,500,Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
//    if (event->oldSize() != QSize(-1,-1))
//    {
//        auto oldSize = event->oldSize();
//        auto newSize = event->size();
//        float ys = newSize.height()/double(oldSize.height());
//        QGraphicsView::setTransform(QTransform::fromScale(ys, ys), true);
//    }
}

void PictureBox::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    for (auto *item : scene->items(group->boundingRect()))
    {
        if (item->group() == group) delete item;
    }
}

void PictureBox::pauseAll()
{
    p_semi->pause();
    n_semi->pause();
    perehod->pause();
    p_semi->group->hide();
    n_semi->group->hide();
    perehod->group->hide();
    halfbit->group->hide();
}

void PictureBox::show_p_semi()
{
    if (p_semi->group->childItems().empty())
    {
        qDebug() << "p draw";
        p_semi->draw();
    }
    pauseAll();
    p_semi->group->show();
    p_semi->unpause();
}

void PictureBox::show_n_semi()
{
    if (n_semi->group->childItems().empty())
    {
        qDebug() << "n draw";
        n_semi->draw();
    }
    pauseAll();
    n_semi->group->show();
    n_semi->unpause();
}

void PictureBox::show_diod()
{
    if (perehod->group->childItems().empty())
    {
        qDebug() << "perehod draw";
        perehod->draw();
    }
    pauseAll();
    perehod->group->show();
    perehod->unpause();
}

void PictureBox::show_halfbitsum()
{
    if (halfbit->group->childItems().empty())
    {
        halfbit->draw();
    }
    pauseAll();
    halfbit->group->show();
}

void PictureBox::callHalfBitSum(bool x, bool y)
{
    halfbit->calculateAndDraw(x,y);
}

void PictureBox::pauseAnim()
{
    p_semi->animation_group->pause();
}

void PictureBox::unpauseAnim()
{
    p_semi->animation_group->resume();
}

void PictureBox::resetAnimations()
{
    p_semi->resetAnimations();
}

QGraphicsTextItem* PictureBox::drawSign(const char c, const QPointF &coords, QGraphicsItemGroup* group)
{
    QGraphicsTextItem *sign = new QGraphicsTextItem({c});
    sign->setFont(QFont("Source Code Pro",28));
    sign->setPos(coords.x(),coords.y());
    sign->setZValue(5);
    group->addToGroup(sign);
    return sign;
}

void PictureBox::changePolarity()
{
    perehod->changePolarity();
}
