#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <QImage>
#include <QPainter> // для пейнт евента

class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap; // для рисования
    QImage image; // текущая картинка
    QImage img1;
    QImage img2;
    void DrawLens(QPainter &painter); // нарисовать линию
    void DrawLens2(QPainter &painter);
public:
    enum class LensNumber
    {
        First,
        Second
    };
    enum class PicNumber
    {
        First,
        Second
    };
    LensNumber Lens = LensNumber::First;
    PicNumber Pic = PicNumber::First;
    explicit PictureBox(QWidget *parent = nullptr);
    void risovanie(); // функция рисования
    void disable();
protected:
    virtual void paintEvent(QPaintEvent*); // функция перерисовки, отрисовывает готовый пиксмап из ф-ции risovanie
};
#endif // PICTUREBOX_H

