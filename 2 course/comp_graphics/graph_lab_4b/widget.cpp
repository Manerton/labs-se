#include "widget.h"
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <GL/glu.h>
#include <fstream>
#include <algorithm>
#include <QMessageBox>


MyWidget::MyWidget(QWidget *parent) : QGLWidget(parent)
{
    resize(700,700); // задаем размеры окна
    paintTimer = new QTimer(this); // создаю таймер
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(repaint()));
    paintTimer->start();
}

void MyWidget::LoadGLTextures()
{
    // Загрузка картинки
    QImage texture1;
    texture1.load(":/files/bricks.jpg");
    texture1 = QGLWidget::convertToGLFormat(texture1);

    // Создание текстуры
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, GLsizei(texture1.width()), GLsizei(texture1.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1.bits());

    texture1.load(":/files/texture1.bmp");
    texture1 = QGLWidget::convertToGLFormat(texture1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, GLsizei(texture1.width()), GLsizei(texture1.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1.bits());
}

struct face {
    struct vertex {
        GLuint v_i;
        GLuint vt_i;
        GLuint vn_i;
    };

    vertex v[3];
    face(vertex v1,vertex v2,vertex v3){
        v[0]=v1;
        v[1]=v2;
        v[2]=v3;
    }
};
using namespace std;

int MyWidget::loadObject(const QString &filename)
{
    vector<string> coord;
    vector<QVector3D> vertex;
    vector<QVector2D> uvs;
    vector<QVector3D> normals;
    vector<face> faces;
    //ifstream in(filename.toStdString());
    QFile in(filename);

    if(!in.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "could not open file for read";
        return -1;
    }
    /*if(!in.is_open())
    {
        qDebug() << "File loaded ploho " << filename ;
        return -1;
    }*/

    char buf[256];

    while(!in.atEnd())
    {
        in.readLine(buf,256);
        coord.push_back(buf);
    }
    uint64_t N_coord = coord.size();
    for (size_t i = 0; i < N_coord; ++i)
    {
        if(coord[i][0] != '#')
        {
            if (coord[i][0] == 'v' && coord[i][1] == ' ') // если вершина
            {
                float tmpx,tmpy,tmpz;
                sscanf(coord[i].c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);
                vertex.push_back(QVector3D(tmpx,tmpy,tmpz));
            } else if (coord[i][0] =='v' && coord[i][1]=='n') // если нормаль
            {
                float tmpx,tmpy,tmpz;
                sscanf(coord[i].c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
                normals.push_back(QVector3D(tmpx,tmpy,tmpz));
            } else if (coord[i][0] =='v' && coord[i][1]=='t') // если текстурная вершина
            {
                float tmpx,tmpy;
                sscanf(coord[i].c_str(),"vt %f %f",&tmpx,&tmpy);
                uvs.push_back(QVector2D(tmpx,tmpy));
            } else if(coord[i][0] == 'f')     // если грань
            {
                face::vertex v1, v2, v3;
                //sscanf(coord[i].c_str(),"f %i//%i %i//%i %i//%i", &v1.v_i, &v1.vn_i, &v2.v_i, &v2.vn_i, &v3.v_i, &v3.vn_i);
                sscanf(coord[i].c_str(),"f %i/%i/%i %i/%i/%i %i/%i/%i", &v1.v_i, &v1.vt_i, &v1.vn_i, &v2.v_i, &v2.vt_i, &v2.vn_i, &v3.v_i, &v3.vt_i, &v3.vn_i);
                faces.push_back(face(v1,v2,v3));     //read in, and add to the end of the face list
            }
        }
    }
    GLuint num = glGenLists(1);
    glNewList(num,GL_COMPILE);
    uint64_t N_faces = faces.size();
    for(size_t i=0; i < N_faces; ++i)
    {
        face f = faces[i];

        glBegin(GL_TRIANGLES);
        QVector3D normal = normals[f.v[0].vn_i - 1];
        glNormal3f(normal.x(), normal.y(), normal.z());
        //glTexCoord2f(0.5f*(normal.x()+1), 0.5f*(normal.y()+1));
        glTexCoord2f(uvs[f.v[0].vt_i - 1].x(), uvs[f.v[0].vt_i - 1].y()); glVertex3f(vertex[f.v[0].v_i - 1].x(),vertex[f.v[0].v_i - 1].y(),vertex[f.v[0].v_i - 1].z());
        normal = normals[f.v[1].vn_i - 1];
        glNormal3f(normal.x(), normal.y(), normal.z());
        //glTexCoord2f(0.5f*(normal.x()+1), 0.5f*(normal.y()+1));
        glTexCoord2f(uvs[f.v[1].vt_i - 1].x(), uvs[f.v[1].vt_i - 1].y()); glVertex3f(vertex[f.v[1].v_i - 1].x(),vertex[f.v[1].v_i - 1].y(),vertex[f.v[1].v_i - 1].z());
        normal = normals[f.v[2].vn_i - 1];
        glNormal3f(normal.x(), normal.y(), normal.z());
        //glTexCoord2f(0.5f*(normal.x()+1), 0.5f*(normal.y()+1));
        glTexCoord2f(uvs[f.v[2].vt_i - 1].x(), uvs[f.v[2].vt_i - 1].y()); glVertex3f(vertex[f.v[2].v_i - 1].x(),vertex[f.v[2].v_i - 1].y(),vertex[f.v[2].v_i - 1].z());
        glEnd();
    }
    glEndList();
    return int(num);     //return with the id
}

int MyWidget::getCube()
{
    GLuint num = glGenLists(1);
    glNewList(num,GL_COMPILE);

    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, 1.0f);     // Нормаль указывает на наблюдателя
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Верх лево

                    // Задняя грань
    glNormal3f( 0.0f, 0.0f,-1.0f);     // Нормаль указывает от наблюдателя
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Низ лево

                    // Верхняя грань
    glNormal3f( 0.0f, 1.0f, 0.0f);     // Нормаль указывает вверх
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Верх право

                    // Нижняя грань
    glNormal3f( 0.0f,-1.0f, 0.0f);     // Нормаль указывает вниз
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Низ право

                    // Правая грань
    glNormal3f( 1.0f, 0.0f, 0.0f);     // Нормаль указывает вправо
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Низ лево

                    // Левая грань
    glNormal3f(-1.0f, 0.0f, 0.0f);     // Нормаль указывает влево
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Верх лево
    glEnd();

    glEndList();
    return num;
}

void MyWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_F) {
        if (texture_count == 0) ++texture_count;
        else --texture_count;
    }
}

void MyWidget::initLight()
{
    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 0.0, 0.0, 2.0, 1.0 };

    /* устанавливаем параметры источника света */
    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);

    /* включаем освещение и источник света */
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
}

void MyWidget::initializeGL()
{
    glEnable(GL_MULTISAMPLE); // сглаживание MSAA вкл
    LoadGLTextures();			// Загрузка текстур
    glEnable(GL_TEXTURE_2D);		// Разрешение наложение текстуры
    //glShadeModel(GL_SMOOTH); // Разрешить плавное цветовое сглаживание
    qglClearColor(Qt::white); // заполняем экран белым цветом
    glClearDepth(1.0); // Разрешить очистку буфера глубины
    glEnable(GL_DEPTH_TEST); // Разрешить тест глубины
    glDepthFunc(GL_LESS); // Тип теста глубины
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Улучшение в вычислении перспективы
    initLight(); // включаем свет
    model = loadObject(":/files/monkey2.obj");
}

void MyWidget::resizeGL(int nWidth, int nHeight)
{
    // установка точки обзора
    glViewport(0, 0, nWidth, nHeight);
    qreal aspectratio = qreal(nWidth) / qreal(nHeight);

    // инициализация матрицы проекций
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // сброс матрицы проекции
    gluPerspective( 90.0, aspectratio, 0.1, 100.0 );

    // инициализация матрицы вида модели
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // сброс матрицы вида модели
}

void MyWidget::paintGL() // рисование
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка экрана и буфера глубины
    glLoadIdentity();           // сбросить текущую матрицу
    glTranslatef(0.0,0.0,-2.5);
    glRotatef(angle,0.0f,1.0f,0.0f);
    glBindTexture(GL_TEXTURE_2D, texture[texture_count]);
    glCallList(model);

    angle += 0.3f;
}


