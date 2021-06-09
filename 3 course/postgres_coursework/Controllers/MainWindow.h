#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include "Models/Database.h"
#include "AuthForm.h"
#include "BuyerForm.h"
#include "ManagerForm.h"
#include "OperatorForm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
// поля
    Ui::MainWindow *ui;
    Database db;
    std::unique_ptr<AuthForm> authForm;
    std::unique_ptr<BuyerForm> buyerForm;
    std::unique_ptr<ManagerForm> managerForm;
    std::unique_ptr<OperatorForm> operatorForm;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showManagerForm();
private slots:
    void showBuyerForm();

    void showOperatorForm();
};
#endif // MAINWINDOW_H
