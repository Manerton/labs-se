#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QTimer> // таймер
#include <QKeyEvent> // для перехвата нажатий клавиш
#include "objloader.h" // загрузчик моделей в формате obj
class MyWidget : public QOpenGLWidget
{
    Q_OBJECT
    QTimer *paintTimer; // таймер
    GLubyte texture_count = 0; // номер текущей текстуры
    GLubyte model_count = 0; // номер текущей модели
    GLuint model[3]; // номер display list для вывода нужной модели
    GLuint torus;
    GLuint texture[3]; // текстуры
    GLfloat angle = 0; // угол вращения

    void initLight(); // включить свет
    GLuint drawCube(); // нарисовать куб
    void LoadGLTextures(); // загрузить текстуры
    void initTexture(uint index, QImage &texture1); // поставить настройки для текстуры
    int loadObject(const QString &filename);
    virtual void keyPressEvent(QKeyEvent *event);


public:
    MyWidget(QWidget *parent = nullptr);
protected:
   void initializeGL();
   void resizeGL(int nWidth, int nHeight);
   void paintGL();
};
#endif // WIDGET_H


