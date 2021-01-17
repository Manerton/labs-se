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
    void getAttributesList_client();
    void getAttributesList_worker();
// поля
    Ui::MainWindow *ui;
    Database db;
private slots:

    void on_pushButton_product_clicked();
    void on_pushButton_client_clicked();
    void on_pushButton_worker_clicked();
    void on_comboBox_client_currentIndexChanged(int index);
    void on_comboBox_worker_currentIndexChanged(int index);
    void on_lineEdit_product_textChanged(const QString &arg1);
    void on_pushButton_date_clicked();
    void on_dateEdit_date_userDateChanged(const QDate &date);
    void on_pushButton_toSold_clicked();
    void on_pushButton_profit_clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
