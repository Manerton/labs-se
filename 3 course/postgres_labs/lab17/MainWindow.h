#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "Database.h"
#include "NagruzkaRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
// методы
    NagruzkaRepository::NagruzkaModel parse_NagruzkaModel();
    void updateAttributesList();
    void clearFields();
    void clearIdField();
// поля
    Ui::MainWindow *ui;
    Database db;
    NagruzkaRepository nagruzkaRepository;
private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_search_clicked();
    void on_tableView_activated(const QModelIndex &index);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
