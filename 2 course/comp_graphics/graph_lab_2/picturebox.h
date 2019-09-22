#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <QPainter> // для пейнт евента
#include <QTimer>
#include <QPicture>

class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap;
    QPicture m_Picture;
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void risovanie();
    void reset();
    bool trail = false;
protected:
    virtual void paintEvent(QPaintEvent*); // функция, которая рисует что либо
};
#endif // PICTUREBOX_H

