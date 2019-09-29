#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <QPainter> // для пейнт евента
#include <QPicture>


class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap; // для рисования со следами
    QPicture m_Picture; // без следов
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void risovanie();
    void reset(); // очищает pixmap от старых следов при вызове
    bool trail = false;

protected:
    virtual void paintEvent(QPaintEvent*); // функция, которая рисует что либо
};
#endif // PICTUREBOX_H

