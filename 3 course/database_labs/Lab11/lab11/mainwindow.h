#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "database.h"
#include <QMainWindow>

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
    void on_pushButton_insert_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_erase_clicked();

    void on_pushButton_search_clicked();

private:
// методы
    Database::tableElem parseTableElem();
// поля
    Ui::MainWindow *ui;
    Database db;
};
#endif // MAINWINDOW_H
