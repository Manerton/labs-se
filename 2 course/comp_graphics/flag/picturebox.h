#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QWidget>
#include <QFrame>
#include <QPainter>

class picturebox : public QFrame
{
    Q_OBJECT
public:
    explicit picturebox(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *);
};

#endif // PICTUREBOX_H
