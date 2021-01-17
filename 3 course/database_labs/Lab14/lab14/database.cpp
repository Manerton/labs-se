#include "database.h"
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <cmath>
#include <map>

// настройки подключения к базе данных
QSqlDatabase Database::createConnection(std::string_view str)
{
    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setDatabaseName(str.cbegin());
    database.setHostName("localhost");
    database.setPort(port);
    database.setUserName("postgres");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Ошибка",
            "Не удалось подключиться к базе данных", QMessageBox::Cancel);
    }
    return database;
}


// вернуть указатель на модель (для интерфейса)
std::shared_ptr<QSqlQueryModel> Database::getModel()
{
    return model;
}

// если строка пустая, то возвращаем строку NULL
QString Database::convertIfNull(const QString &elem)
{
    if (elem.trimmed().isEmpty()) return "NULL";
    return elem;
}

QString Database::quote(const QString &str) const
{
    return ("\"" + str + "\"");
}

QString Database::singleQuote(const QString &str) const
{
    return ("'" + str + "'");
}

QString Database::vec_to_str(const Tokens &v, char delim) const
{
    QString result;
    for (auto &str : v)
    {
        result += delim + str;
    }
    return result;
}

std::map<int32_t, QString> Database::getAttributesList(const QString &str)
{
    Tokens args;
    if (str == "Клиент")
    {
        args = {"select * from get_clients_fio()"};
    }
    else if (str == "Сотрудник")
    {
        args = {"select * from get_workers_fio()"};
    }
    std::map<int32_t, QString> map;
    if (!query.exec(vec_to_str(args)))
    {
        QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    }
    else
    {
        while (query.next())
        {
            map[query.value(0).toInt()] = query.value(1).toString();
        }
    }
    return map;
}

void Database::searchByProduct(const QString &str)
{
    Tokens args = {"SELECT * FROM show_by_product(", singleQuote(str),")"};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else model->setQuery(query);
}

void Database::searchByClient(const int32_t &id)
{
    // нам придет ID = 0, если пользователь ничего не выбрал
    // тогда не будем ничего делать
    if (id != 0)
    {
        QString str = QString::number(id);
        Tokens args = {"SELECT * FROM show_by_client(", str,")"};
        if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
        else model->setQuery(query);
    }
}

void Database::searchByWorker(const int32_t &id)
{
    // нам придет ID = 0, если пользователь ничего не выбрал
    // тогда не будем ничего делать
    if (id != 0)
    {
        QString str = QString::number(id);
        Tokens args = {"SELECT * FROM show_by_worker(", str,")"};
        if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
        else model->setQuery(query);
    }
}

void Database::searchByDate(const QString &str)
{
    Tokens args = {"SELECT * FROM show_by_date(", singleQuote(str),")"};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else model->setQuery(query);
}

void Database::showExpired(const QString &start, const QString &end)
{
    Tokens args = {"SELECT * FROM show_expired(", singleQuote(start), ",", singleQuote(end),")"};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else model->setQuery(query);
}

void Database::showProfit(const QString &start, const QString &end)
{
    Tokens args = {"SELECT * FROM calc_profit(", singleQuote(start), ",", singleQuote(end),")"};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else model->setQuery(query);
}
