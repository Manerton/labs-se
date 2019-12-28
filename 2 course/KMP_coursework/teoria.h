#ifndef TEORIA_H
#define TEORIA_H

#include <QWidget>

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
    void on_textBrowser_anchorClicked(const QUrl &arg1);

    void on_pushButton_clicked();
private:
    Ui::Teoria *ui;

};

#endif // TEORIA_H
