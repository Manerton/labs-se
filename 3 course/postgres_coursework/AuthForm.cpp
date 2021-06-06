#include "AuthForm.h"
#include "ui_AuthForm.h"

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
    emit authBuyerDone();
    this->close();
}


void AuthForm::on_auth_pushButton_clicked()
{
    QString login = ui->login_LineEdit->text();
    QString pass = ui->pass_LineEdit->text();
    db.setConnection(db_name, login.toStdString(), pass.toStdString());
    db.exec("SELECT whoami()");
    db.first();
    const auto role = db.value(0).toString();
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

AuthForm::~AuthForm()
{
    delete ui;
}


void AuthForm::on_pass_LineEdit_returnPressed()
{
    ui->auth_pushButton->click();
}

