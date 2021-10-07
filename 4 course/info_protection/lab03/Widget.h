#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    using permissions_t = uint8_t;

    enum object_permissions : permissions_t
    {
        no = 0,
        read = 1,
        modify = 2
    };

    enum admin_permissions : permissions_t
    {
        no_admin = 0,
        yes_admin = 1
    };

    using Row = std::vector<permissions_t>;
    using Matrix = std::vector<Row>;

    struct Object
    {
        QString name;
        QString content;
    };

    Ui::Widget *ui;

    /// Массив пользователей
    std::vector<QString> Users;

    /// Массив значений объектов
    std::vector<Object> Objects;

    /// Матрица прав доступа
    Matrix P;

    /// Добавить пользователя
    size_t createUser(const QString &name, admin_permissions p);
    /// Удалить пользователя
    void deleteUser(size_t i);

    /// Добавить объект
    size_t createObject(const Object &obj);
    /// Удалить объект
    void deleteObject(size_t j);

    /// Получить права на объект j для юзера i
    object_permissions getObjectPermissionForUser(size_t i, size_t j) const;
    /// Установить права на объект j для юзера i
    void setObjectPermissionForUser(size_t i, size_t j, object_permissions p);

    /// Юзер i является админом?
    bool userIsAdmin(size_t i) const;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_comboBox_user_currentIndexChanged(int index);
    void on_listWidget_files_currentRowChanged(int currentRow);
    void on_textBrowser_file_textChanged();
    void on_pushButton_back_clicked();
    void on_pushButton_editPermission_clicked();
    void on_pushButton_addUser_clicked();
    void on_pushButton_addObject_clicked();
    void on_pushButton_deleteUser_clicked();
    void on_pushButton_deleteObject_clicked();
    void on_comboBox_user_2_currentIndexChanged(int index);
    void on_comboBox_object_currentIndexChanged(int index);
    void on_pushButton_addPermission_clicked();
};
#endif // WIDGET_H
