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
    struct tableElem {
        uint32_t ID;
        uint16_t eduYear;
        QString prepod;
        uint32_t groupID;
        uint16_t hours;
        QString lesson;
        double hourCost;
    };
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
        this->select();
    }

    ~Database() {
        db.close();
    }
    std::shared_ptr<QSqlQueryModel> getModel();
    void select();
    void insert(const tableElem& elem, const QString &tableName = "Нагрузка");
    void update(const tableElem& elem, const QString &tableName = "Нагрузка");
    void erase(const tableElem& elem, const QString &tableName = "Нагрузка");
    void search(const tableElem& elem, const QString &tableName = "Нагрузка");
    void bindValues(const tableElem& elem, QSqlQuery& query);
    template <typename T>
    QVariant convertIfNull(const T& elem);
};

#endif // DATABASE_H
