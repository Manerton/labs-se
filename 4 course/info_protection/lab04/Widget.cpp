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

    // сделаем кнопку 2, 5 и 6 не активными
    ui->pushButton_toEditPermissionsPage->setEnabled(false);
    ui->pushButton_copy->setEnabled(false);
    ui->pushButton_paste->setEnabled(false);

    // добавляем юзера админа с маскимальными привилегиями
    auto i1 = createUser("Пользователь1", AdminPermissions::yes_admin);
    privilegesUser[i1] = Privileges::top_secret;

    // и добавляем обычного юзера с привилегиями confidential
    auto i2 = createUser("Пользователь2", AdminPermissions::no_admin);
    privilegesUser[i2] = Privileges::confidential;

    // добавим четыре объекта
    auto j1 = createObject({"объект1", "пример1"});
    auto j2 = createObject({"объект2", "пример2"});
    auto j3 = createObject({"объект3", "пример3"});
    auto j4 = createObject({"объект4", "пример4"});

    /// установим разные привилегии
    // первый объект при создании автоматически создан с привилегией "несекретно"
    // поэтому не будем это указывать явно
    privilegesObject[j2] = Privileges::confidential;
    privilegesObject[j3] = Privileges::secret;
    privilegesObject[j4] = Privileges::top_secret;

    // последний объект при создании автоматически создан с правами доступа "нет прав"
    // поэтому не будем это указывать явно
    this->setObjectPermissionForUser(i1, j1, ObjectPermissions::modify);
    this->setObjectPermissionForUser(i1, j2, ObjectPermissions::modify);
    this->setObjectPermissionForUser(i1, j3, ObjectPermissions::read);
    this->setObjectPermissionForUser(i1, j4, ObjectPermissions::read);

    // для второго юзера
    this->setObjectPermissionForUser(i2, j1, ObjectPermissions::modify);
    this->setObjectPermissionForUser(i2, j2, ObjectPermissions::modify);
    this->setObjectPermissionForUser(i2, j3, ObjectPermissions::read);

    // варианты прав доступа в окне редактирования прав доступа и привилегий
    ui->comboBox_permission->addItem("Нет прав");
    ui->comboBox_permission->addItem("Чтение");
    ui->comboBox_permission->addItem("Чтение и запись");

    // варианты привилегий в окне редактирования прав доступа и привилегий
    QStringList items = {"Несекретно", "Конфиденциально", "Секретно", "Совершенно секретно"};
    ui->comboBox_priviligesUser->addItems(items);
    ui->comboBox_priviligesObject->addItems(items);
}

Widget::~Widget()
{
    delete ui;
}

size_t Widget::createUser(const QString& name, AdminPermissions p)
{
    const auto i = users.size();
    // добавляем юзера в массив
    users.emplace_back(name);
    // добавляем новую строку в матрицу доступа
    const auto M = objects.size();
    P.emplace_back(std::vector<permissions_t>(M+1));
    // указываем административные права
    P[i][0] = p;

    // установим права юзера для всех файлов на "нет прав"
    for (size_t j = 0; j < M; ++j)
    {
        this->setObjectPermissionForUser(i,j,ObjectPermissions::no);
    }

    // укажем привилегию "несекретно" для нового юзера
    privilegesUser.emplace_back(Privileges::unclassified);

    // добавляем нового юзера в список юзеров на форме
    ui->comboBox_user->addItem(name);

    // и на форме админа
    ui->comboBox_user_2->addItem(name);

    return (users.size() - 1);
}

void Widget::deleteUser(size_t i)
{
    // удаляем из массива
    users.erase(users.begin() + ptrdiff_t(i));

    // удаляем строку в матрице доступа
    P.erase(P.begin() + ptrdiff_t(i));

    // удалим привилегию юзера
    privilegesUser.erase(privilegesUser.begin() + ptrdiff_t(i));

    // и удаляем с форм
    ui->comboBox_user->removeItem(int(i));
    ui->comboBox_user_2->removeItem(int(i));
}

size_t Widget::createObject(const Object &obj)
{
    // добавляем объект в массив
    objects.emplace_back(obj);

    // для каждой строки матрицы доступа добавим нулевые права на файл
    for (auto &row : P) row.emplace_back(ObjectPermissions::no);

    // укажем привилегию "несекретно" для нового объекта
    privilegesObject.emplace_back(Privileges::unclassified);

    // добавляем новый объект в список объектов на форме
    ui->listWidget_files->addItem(new QListWidgetItem(obj.name));

    // и на форме админа
    ui->comboBox_object->addItem(obj.name);

    return (objects.size() - 1);
}

void Widget::deleteObject(size_t j)
{
    objects.erase(objects.begin() + ptrdiff_t(j));

    // удаляем столбец в матрице доступа
    // не забываем, что объекты индексируются с 1, а не с 0
    for (auto &row : P) row.erase(row.begin() + ptrdiff_t(j+1));

    // удалим привилегию объекта
    privilegesObject.erase(privilegesObject.begin() + ptrdiff_t(j));

    // и удаляем с форм
    delete ui->listWidget_files->item(int(j));
    ui->comboBox_object->removeItem(int(j));
}

Widget::ObjectPermissions Widget::getObjectPermissionForUser(size_t i, size_t j) const
{
    return ObjectPermissions(P[i][j+1]);
}

void Widget::setObjectPermissionForUser(size_t i, size_t j, ObjectPermissions p)
{
    P[i][j+1] = p;
}

bool Widget::userIsAdmin(size_t i) const
{
    return P[i][0];
}

void Widget::on_comboBox_user_currentIndexChanged(int i)
{

    ui->pushButton_toEditPermissionsPage->setEnabled(userIsAdmin(size_t(i)));

    // загрузим выбранный файл с соответствии с новым юзером
    auto j = ui->listWidget_files->currentRow();
    if (j != -1)
    {
        this->on_listWidget_files_currentRowChanged(j);

        // выполняем действия согласно матрице привилегий
        ui->pushButton_copy->setEnabled(privilegesUser[size_t(i)] > privilegesObject[size_t(j)]);

        this->buffer.clear();
        ui->pushButton_paste->setEnabled(false);
    }
}

void Widget::on_listWidget_files_currentRowChanged(int row)
{
    auto i = size_t(ui->comboBox_user->currentIndex());
    auto j = size_t(row);

    const auto hideCopyPaste = [&](bool hide = true)
    {
        ui->pushButton_copy->setHidden(hide);
        ui->pushButton_paste->setHidden(hide);
    };

    QString content = "";
    bool readOnly = true;
    if (getObjectPermissionForUser(i,j) == ObjectPermissions::no
            || privilegesUser[i] < privilegesObject[j])
    {
        hideCopyPaste();
    }
    else if (getObjectPermissionForUser(i,j) == ObjectPermissions::read)
    {
        content = objects[j].content;
        hideCopyPaste(false);
    }
    else if (getObjectPermissionForUser(i,j) == ObjectPermissions::modify)
    {
        content = objects[j].content;
        readOnly = false;
        hideCopyPaste(false);
    }
    ui->textBrowser_file->setReadOnly(readOnly);
    ui->textBrowser_file->setText(content);

    // выполняем действия согласно матрице привилегий
    ui->pushButton_copy->setEnabled(privilegesUser[i] > privilegesObject[j]);

    const bool enablePasteButton = (!this->buffer.isEmpty()
                                    && privilegesBuffer < privilegesObject[j]);
    ui->pushButton_paste->setEnabled(enablePasteButton);
}

void Widget::on_textBrowser_file_textChanged()
{
    if (ui->comboBox_user->count() > 0 && ui->listWidget_files->count() > 0)
    {
        auto i = size_t(ui->comboBox_user->currentIndex());
        auto j = size_t(ui->listWidget_files->currentRow());

        if (getObjectPermissionForUser(i,j) == ObjectPermissions::modify)
        {
            objects[j].content = ui->textBrowser_file->toPlainText();
        }
    }
}

void Widget::on_pushButton_toEditPermissionsPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    this->on_comboBox_user_2_currentIndexChanged(ui->comboBox_user_2->currentIndex());
    this->on_comboBox_object_currentIndexChanged(ui->comboBox_object->currentIndex());
}

void Widget::on_pushButton_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    // загрузим выбранный файл с соответствии с новыми правами
    auto j = ui->listWidget_files->currentRow();
    if (j != -1) this->on_listWidget_files_currentRowChanged(j);
}

void Widget::on_pushButton_addUser_clicked()
{
    this->createUser(ui->lineEdit_user->text(), AdminPermissions(ui->checkBox_admin->isChecked()));
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

    ObjectPermissions permission = this->getObjectPermissionForUser(i,j);
    ui->comboBox_permission->setCurrentIndex(permission);

    Privileges p = this->privilegesUser[i];
    ui->comboBox_priviligesUser->setCurrentIndex(p);
}

void Widget::on_comboBox_object_currentIndexChanged(int index)
{
    auto j = size_t(index);
    auto i = size_t(ui->comboBox_user_2->currentIndex());

    ObjectPermissions permission = this->getObjectPermissionForUser(i,j);
    ui->comboBox_permission->setCurrentIndex(permission);

    Privileges p = this->privilegesObject[j];
    ui->comboBox_priviligesObject->setCurrentIndex(p);
}

void Widget::on_pushButton_editPermission_clicked()
{
    auto i = size_t(ui->comboBox_user_2->currentIndex());
    auto j = size_t(ui->comboBox_object->currentIndex());
    auto p = ObjectPermissions(ui->comboBox_permission->currentIndex());
    this->setObjectPermissionForUser(i,j,p);
}

void Widget::on_pushButton_editPriviligesUser_clicked()
{
    auto i = size_t(ui->comboBox_user_2->currentIndex());
    auto p = Privileges(ui->comboBox_priviligesUser->currentIndex());
    this->privilegesUser[i] = p;
}

void Widget::on_pushButton_editPriviligesObject_clicked()
{
    auto j = size_t(ui->comboBox_object->currentIndex());
    auto p = Privileges(ui->comboBox_priviligesObject->currentIndex());
    this->privilegesObject[j] = p;
}

void Widget::on_pushButton_copy_clicked()
{
    auto j = size_t(ui->listWidget_files->currentRow());
    this->privilegesBuffer = privilegesObject[j];
    this->buffer = this->objects[j].content;
}

void Widget::on_pushButton_paste_clicked()
{
    auto j = size_t(ui->listWidget_files->currentRow());
    this->objects[j].content = this->buffer;
    this->on_listWidget_files_currentRowChanged(int(j));
}

