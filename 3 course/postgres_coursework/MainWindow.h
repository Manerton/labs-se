#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include "Database.h"
#include "AuthForm.h"
#include "OperatorForm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
// методы
//    Repository::NagruzkaModel parse_NagruzkaModel();
    void updateAttributesList();
    void clearFields();
    void clearIdField();
// поля
    Ui::MainWindow *ui;
    Database db;
    std::unique_ptr<AuthForm> authForm;
    std::unique_ptr<OperatorForm> operatorForm;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void showOperatorForm();
    void showBuyerForm();
};
#endif // MAINWINDOW_H
