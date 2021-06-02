#ifndef DATABASE_H
#define DATABASE_H
#include <memory>
#include <map>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

class Database
{
private:
// поля
    // порт соединения с базой
    static constexpr auto hostname = "localhost";
    static constexpr int port = 5432;
    static constexpr auto username = "postgres";
    static constexpr auto password = "postgres";
    // объект подключения к базе
    QSqlDatabase db;
    // модель базы данных (т.е эту модель необходимо передать интерфейсу в виджет View)
    std::shared_ptr<QSqlQueryModel> model;
    // объект для передачи SQL запросов в базу на исполнение
    // из него же получаем и результаты
    QSqlQuery query;
// методы
    // создать подключение к БД
    QSqlDatabase createConnection(std::string_view str);
public:
    explicit Database(std::string_view db_name)
        : db(createConnection(db_name)),
          model{std::make_shared<QSqlQueryModel>()},
          query(db)
    {}
    // вернуть указатель на модель (для интерфейса)
    std::shared_ptr<QSqlQueryModel> getModel()
    {
        return model;
    }
    //void select(const QString &tableName);
    void exec(const QString &str);
    void execWithDisplay(const QString &str);
    std::map<int32_t, QString> getAttributesList(const QString &str);

};

#endif // DATABASE_H
