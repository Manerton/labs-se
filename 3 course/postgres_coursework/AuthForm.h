#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QWidget>
#include "Database.h"

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
};

#endif // AUTHFORM_H
