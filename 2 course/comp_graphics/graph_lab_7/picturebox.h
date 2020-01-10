#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <vector>
#include <QPainter> // для пейнт евента
#include <stack>
#include <QMouseEvent>

class PictureBox : public QFrame
{
    Q_OBJECT
    QImage m_Pixmap; // для рисования
    QPixmap m_Grid; // сетка
    QVector<QPoint> vertex;
    std::vector<QPoint> intersections;
    std::stack<QPoint> zatravka;
    void sort_intersections();
    void find_local_min_max();
    void fill();
    void fill_figure(QPainter &painter);
    void DrawGrid(); // нарисовать сетку
    void mousePressEvent ( QMouseEvent * event );

    void zatravka_line_poisk_new_zatravka(QPoint p, int x_right); // -- часть алгоритма построчной затравки -- //
    
private slots:
    void DrawDirectLine(const QPoint q1, const QPoint q2, QColor color, QPainter &painter, bool write_points = false);
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void risovanie(); // функция рисования (построчная заливка)
    void risovanie_zatravka(); // функция рисования (простая затравочная)
    void risovanie_zatravka_line(); // функция рисования (затравочная построчная)
protected:
    virtual void paintEvent(QPaintEvent*); // функция перерисовки, отрисовывает готовый пиксмап из ф-ции risovanie
};
#endif // PICTUREBOX_H

