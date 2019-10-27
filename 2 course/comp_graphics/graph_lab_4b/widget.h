#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer> // таймер
#include <QKeyEvent>
class MyWidget : public QGLWidget
{
    Q_OBJECT
    QTimer *paintTimer; // таймер
    GLfloat angle; // угол вращения
    GLuint texture[2]; // текстура
    int texture_count = 0; // номер текущей текстуры
    void initLight();
public:
    MyWidget(QWidget *parent = nullptr);
protected:
   void initializeGL();
   void resizeGL(int nWidth, int nHeight);
   void paintGL();
   void LoadGLTextures();
   virtual void keyPressEvent(QKeyEvent *event);
};
#endif // WIDGET_H


