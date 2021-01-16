#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>
#include <QVariant>
#include <memory>

class Database
{
public:
    // строка таблицы, чтобы удобно передавать из интерфейса (из полей ввода) в этот класс
    struct nagruzka_tableElem {
        QString ID;
        QString prepodID;
        QString groupID;
        QString hours;
        QString lesson;
        QString typeLesson;
        QString pay;
    };
    struct prepod_tableElem {
        QString ID;
        QString familiya;
        QString name;
        QString otchestvo;
        QString phone;
        QString exp;
    };
    struct group_tableElem {
        QString ID;
        QString specialnost;
        QString otdelenie;
        QString count;
    };



    enum class dbTableName
    {
        Nagruzka = 0,
        Prepod = 1,
        Group = 2
    };

    using Tokens = std::vector<QString>;
private:
// поля
    static constexpr int port = 5432; // порт соединения с базой
    // объект подключения к базе
    QSqlDatabase db;
    // модель базы данных (т.е эту модель необходимо передать интерфейсу в виджет View)
    std::shared_ptr<QSqlQueryModel> model;
    QSqlQuery query;
// методы
    QSqlDatabase createConnection(std::string_view str);
public:
    explicit Database(std::string_view str) : db(createConnection(str)), model{std::make_shared<QSqlQueryModel>()}, query(db)
    {
        this->selectCurrent();
    }
    std::shared_ptr<QSqlQueryModel> getModel();
    void selectCurrent();
    void select(const QString &tableName);
    // для таблицы группа
    void insert(const group_tableElem& elem);
    void update(const group_tableElem& elem);
    void erase(const group_tableElem& elem);
    void search(const group_tableElem& elem);
    // для таблицы преподаватель
    void insert(const prepod_tableElem& elem);
    void update(const prepod_tableElem& elem);
    void erase(const prepod_tableElem& elem);
    void search(const prepod_tableElem& elem);
    // для таблицы нагрузка
    void insert(const nagruzka_tableElem& elem);
    void update(const nagruzka_tableElem& elem);
    void erase(const nagruzka_tableElem& elem);
    void search(const nagruzka_tableElem& elem);
    QString quotedStr(const QString &str) const;
    QString singleQuotedStr(const QString &str) const;
    QString vec_to_str(const Tokens &v) const;
    std::vector<size_t> getAttributesList(const QString &str);
    QString convertIfNull(const QString& elem);
    QString get_dbTableName(dbTableName dbTN);
};

#endif // DATABASE_H
