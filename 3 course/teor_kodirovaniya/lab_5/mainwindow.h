#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <set>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_finishText_clicked();
    void on_lineEdit_text_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    void startWork();
    QString str;
};
#endif // MAINWINDOW_H
