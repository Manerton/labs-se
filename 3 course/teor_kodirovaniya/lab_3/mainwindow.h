#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <set>
#include <svgview.h>

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
    void on_pushButton_fromFile_clicked();
    void on_pushButton_finishText_clicked();
private:
    Ui::MainWindow *ui;
    SvgView *m_view;
    // подготовить таблицу с алфавитом сообщения (номер, символ, частота, относит. частота)
    void prepareTable(QTableWidget* table, std::multiset<QString> &set);
    void startWork();
    QString str;
};
#endif // MAINWINDOW_H
