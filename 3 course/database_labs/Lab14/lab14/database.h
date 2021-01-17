#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <memory>
#include <vector>
#include <map>

class Database
{
public:
    using Tokens = std::vector<QString>;
private:
// поля
    // порт соединения с базой
    static constexpr int port = 5432;
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
    // строку взять в двойные кавычки
    QString quote(const QString &str) const;
    // в одинарные
    QString singleQuote(const QString &str) const;
    // слепить токены в строку
    QString vec_to_str(const Tokens &v, char delim = ' ') const;
    // если строка пустая, то возвращаем строку NULL
    QString convertIfNull(const QString& elem);
public:
    explicit Database(std::string_view str)
        : db(createConnection(str)), model{std::make_shared<QSqlQueryModel>()}, query(db)
    { }
    std::shared_ptr<QSqlQueryModel> getModel();
    std::map<int32_t, QString> getAttributesList(const QString &str);
    void searchByProduct(const QString &str);
    void searchByClient(const int32_t &id);
    void searchByWorker(const int32_t &id);
    void searchByDate(const QString &str);
    void showExpired(const QString &start, const QString &end);
    void showProfit(const QString &start, const QString &end);
};

#endif // DATABASE_H
