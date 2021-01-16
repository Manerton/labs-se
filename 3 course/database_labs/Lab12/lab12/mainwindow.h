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



// методы
    Database::nagruzka_tableElem parse_nagruzka_TableElem();
    Database::group_tableElem parse_group_TableElem();
    Database::prepod_tableElem parse_prepod_TableElem();
    void updateAttributesList();
// поля
    Ui::MainWindow *ui;
    Database db;
private slots:
    void on_pushButton_insert_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_erase_clicked();
    void on_pushButton_search_clicked();
    void on_tabWidget_currentChanged(int index);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
