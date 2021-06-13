#include "AuthForm.h"
#include "../Views/ui_AuthForm.h"

AuthForm::AuthForm(Database& _db) :
    QWidget(),
    ui(new Ui::AuthForm),
    db(_db)
{
    ui->setupUi(this);
}

void AuthForm::on_authBuyer_pushButton_clicked()
{
    // подключаемся в базу как покупатель,
    // и закрываем окно авторизации
    db.setConnection(db_name, "buyer", "buyer");
    if (db.isConnected())
    {
        emit authBuyerDone();
        this->close();
    }
}

void AuthForm::on_auth_pushButton_clicked()
{
    QString login = ui->login_LineEdit->text();
    QString pass = ui->pass_LineEdit->text();
    db.setConnection(db_name, login.toStdString(), pass.toStdString());
    if (db.isConnected())
    {
        db.exec("SELECT whoami()");
        const auto role = db.getFirstValue(0).toString();
        if (role == "operator")
        {
            emit authOperatorDone();
        }
        else if (role == "managers")
        {
            emit authManagerDone();
        }
        this->close();
    }
}

void AuthForm::on_pass_LineEdit_returnPressed()
{
    ui->auth_pushButton->click();
}

void AuthForm::on_login_LineEdit_returnPressed()
{
    ui->auth_pushButton->click();
}

AuthForm::~AuthForm()
{
    delete ui;
}
