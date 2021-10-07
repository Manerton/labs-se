#include "Widget.h"
#include "./ui_Widget.h"

#include <cmath>
#include <algorithm>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // сделаем кнопку 2 не активной
    ui->pushButton_editPermission->setEnabled(false);

    // добавляем юзера админа
    createUser("Пользователь1", admin_permissions::yes_admin);

    // и добавляем обычного юзера
    createUser("Пользователь2", admin_permissions::no_admin);

    // добавим три объекта с разными правами для первого юзера
    createObject({"объект1", "пример1"});
    size_t j2 = createObject({"объект2", "пример2"});
    size_t j3 = createObject({"объект3", "пример3"});

    this->setObjectPermissionForUser(0, j2, object_permissions::read);
    this->setObjectPermissionForUser(0, j3, object_permissions::modify);

    // варианты прав доступа
    ui->comboBox_permission->addItem("Нет прав", object_permissions::no);
    ui->comboBox_permission->addItem("Чтение", object_permissions::read);
    ui->comboBox_permission->addItem("Чтение и запись", object_permissions::modify);
}

Widget::~Widget()
{
    delete ui;
}

size_t Widget::createUser(const QString& name, admin_permissions p)
{
    const auto i = Users.size();
    // добавляем юзера в массив
    Users.emplace_back(name);
    // добавляем новую строку в матрицу доступа
    const auto M = Objects.size();
    P.emplace_back(std::vector<permissions_t>(M+1));
    // указываем административные права
    P[i][0] = p;

    // установим права юзера для всех файлов на "нет прав"
    for (size_t j = 0; j < M; ++j)
    {
        this->setObjectPermissionForUser(i,j,object_permissions::no);
    }

    // добавляем нового юзера в список юзеров на форме
    ui->comboBox_user->addItem(name);

    // и на форме админа
    ui->comboBox_user_2->addItem(name);

    return (Users.size() - 1);
}

void Widget::deleteUser(size_t i)
{
    // удаляем из массива
    Users.erase(Users.begin() + ptrdiff_t(i));

    // удаляем строку в матрице доступа
    P.erase(P.begin() + ptrdiff_t(i));

    // и удаляем с форм
    ui->comboBox_user->removeItem(int(i));
    ui->comboBox_user_2->removeItem(int(i));
}

size_t Widget::createObject(const Object &obj)
{
    // добавляем объект в массив
    Objects.emplace_back(obj);

    // для каждой строки матрицы доступа добавим нулевые права на файл
    for (auto &row : P) row.emplace_back(object_permissions::no);

    // добавляем новый объект в список объектов на форме
    ui->listWidget_files->addItem(new QListWidgetItem(obj.name));

    // и на форме админа
    ui->comboBox_object->addItem(obj.name);

    return (Objects.size() - 1);
}

void Widget::deleteObject(size_t j)
{
    Objects.erase(Objects.begin() + ptrdiff_t(j));

    // удаляем столбец в матрице доступа
    // не забываем, что объекты индексируются с 1, а не с 0
    for (auto &row : P) row.erase(row.begin() + ptrdiff_t(j+1));

    // и удаляем с форм
    qDebug() << j;
    delete ui->listWidget_files->item(int(j));
    ui->comboBox_object->removeItem(int(j));
}

Widget::object_permissions Widget::getObjectPermissionForUser(size_t i, size_t j) const
{
    return object_permissions(P[i][j+1]);
}

void Widget::setObjectPermissionForUser(size_t i, size_t j, object_permissions p)
{
    P[i][j+1] = p;
}

bool Widget::userIsAdmin(size_t i) const
{
    return P[i][0];
}

void Widget::on_comboBox_user_currentIndexChanged(int i)
{

    ui->pushButton_editPermission->setEnabled(userIsAdmin(size_t(i)));

    // загрузим выбранный файл с соответствии с новым юзером
    auto j = ui->listWidget_files->currentRow();
    if (j != -1) this->on_listWidget_files_currentRowChanged(j);
}

void Widget::on_listWidget_files_currentRowChanged(int row)
{
    auto i = size_t(ui->comboBox_user->currentIndex());
    auto j = size_t(row);

    QString content = "";
    bool readOnly = true;
    if (getObjectPermissionForUser(i,j) == object_permissions::read)
    {
        content = Objects[j].content;
    }
    else if (getObjectPermissionForUser(i,j) == object_permissions::modify)
    {
        content = Objects[j].content;
        readOnly = false;
    }
    ui->textBrowser_file->setReadOnly(readOnly);
    ui->textBrowser_file->setText(content);
}

void Widget::on_textBrowser_file_textChanged()
{
    if (ui->comboBox_user->count() > 0 && ui->listWidget_files->count() > 0)
    {
        auto i = size_t(ui->comboBox_user->currentIndex());
        auto j = size_t(ui->listWidget_files->currentRow());

        if (getObjectPermissionForUser(i,j) == object_permissions::modify)
        {
            Objects[j].content = ui->textBrowser_file->toPlainText();
        }
    }
}

void Widget::on_pushButton_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    // загрузим выбранный файл с соответствии с новыми правами
    auto j = ui->listWidget_files->currentRow();
    if (j != -1) this->on_listWidget_files_currentRowChanged(j);
}

void Widget::on_pushButton_editPermission_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_pushButton_addUser_clicked()
{
    this->createUser(ui->lineEdit_user->text(), admin_permissions(ui->checkBox_admin->isChecked()));
}

void Widget::on_pushButton_addObject_clicked()
{
    this->createObject({ui->lineEdit_object->text(), ""});
}

void Widget::on_pushButton_deleteUser_clicked()
{
    auto i = ui->comboBox_user_2->currentIndex();
    this->deleteUser(size_t(i));
}

void Widget::on_pushButton_deleteObject_clicked()
{
    auto j = ui->comboBox_object->currentIndex();
    this->deleteObject(size_t(j));
}

void Widget::on_comboBox_user_2_currentIndexChanged(int index)
{
    auto i = size_t(index);
    auto j = size_t(ui->comboBox_object->currentIndex());
    int p = this->getObjectPermissionForUser(i,j);
    ui->comboBox_permission->setCurrentIndex(p);
}

void Widget::on_comboBox_object_currentIndexChanged(int index)
{
    auto j = size_t(index);
    auto i = size_t(ui->comboBox_user_2->currentIndex());
    int p = this->getObjectPermissionForUser(i,j);
    ui->comboBox_permission->setCurrentIndex(p);
}

void Widget::on_pushButton_addPermission_clicked()
{
    auto i = size_t(ui->comboBox_user_2->currentIndex());
    auto j = size_t(ui->comboBox_object->currentIndex());
    auto p = object_permissions(ui->comboBox_permission->currentIndex());
    this->setObjectPermissionForUser(i,j,p);
}

