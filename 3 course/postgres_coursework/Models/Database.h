#ifndef DATABASE_H
#define DATABASE_H
#include <memory>
#include <map>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

using std::string_view;

class Database
{
// поля
    // настройки подключения к БД
    static constexpr auto hostname = "localhost";
    static constexpr int port = 5432;
    // объект подключения к базе
    QSqlDatabase db;
    // модель базы данных (т.е эту модель необходимо передать интерфейсу в виджет View)
    std::shared_ptr<QSqlQueryModel> model;
    // объект для передачи SQL запросов в базу на исполнение
    // из него же получаем и результаты
    QSqlQuery query;
    bool connected = false;
// методы
public:
    explicit Database()
        : db(QSqlDatabase::addDatabase("QPSQL")),
          model{std::make_shared<QSqlQueryModel>()},
          query(db)
    {}
    // установить подключение к БД
    void setConnection(string_view db_name, string_view login, string_view password);
    bool isConnected() const;
    // вернуть указатель на модель (для интерфейса)
    std::shared_ptr<QSqlQueryModel> getModel()
    {
        return model;
    }
    // эти методы - обертка методов объекта query (что-то типа декоратора)
    bool exec();
    bool exec(const QString &str);
    bool execWithDisplay(const QString &str);
    bool first();
    bool next();
    QVariant value(int i);
    bool prepare(const QString &str);
    void bindValue(const QString &placeholder, const QVariant &val);
    std::map<int, QString> getAttributesList(const QString &str);
    bool transaction();
    bool commit();
    bool rollback();
};

#endif // DATABASE_H
