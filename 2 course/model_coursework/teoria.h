#ifndef TEORIA_H
#define TEORIA_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTextBrowser>

namespace Ui {
class Teoria;
}

class Teoria : public QWidget
{
    Q_OBJECT

public:
    explicit Teoria(QWidget *parent = nullptr);
    ~Teoria();

signals:
    void return_to_menu();
private slots:
    void on_pushButton_clicked();
    void on_listWidget_clicked(const QModelIndex &index);
private:
    Ui::Teoria *ui;
    void fillContentTable();

};

#endif // TEORIA_H
