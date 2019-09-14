#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

class PictureBox : public QFrame
{
    Q_OBJECT
public:
    explicit PictureBox(QWidget *parent = nullptr);
protected:
    virtual void paint(QPaintEvent*);
};

#endif // MAINWINDOW_H
