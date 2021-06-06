#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QWidget>
#include "Models/Database.h"

namespace Ui {
    class AuthForm;
}

class AuthForm : public QWidget
{
    Q_OBJECT
    Ui::AuthForm *ui;
    static constexpr auto db_name = "online_shop";
    Database& db;
public:
    explicit AuthForm(Database& _db);
    ~AuthForm();

signals:
    void authBuyerDone();
    void authOperatorDone();
    void authManagerDone();

private slots:
    void on_authBuyer_pushButton_clicked();
    void on_auth_pushButton_clicked();
    void on_pass_LineEdit_returnPressed();
};

#endif // AUTHFORM_H
