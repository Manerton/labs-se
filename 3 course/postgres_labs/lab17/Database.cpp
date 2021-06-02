#include "Database.h"
#include <cmath>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include "StringTools.h"

using namespace StringTools;

// подключение к БД
QSqlDatabase Database::createConnection(std::string_view db_name)
{
    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setDatabaseName(db_name.cbegin());
    database.setHostName(hostname);
    database.setPort(port);
    database.setUserName(username);
    database.setPassword(password);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "Ошибка",
            "Не удалось подключиться к базе данных:\n" + database.lastError().text(), QMessageBox::Close);

        exit(EXIT_FAILURE);
    }

    return database;
}

/*// вывод таблицы tableName в модель
void Database::select(const QString &tableName)
{
    const Tokens args = {"SELECT * FROM", quote(tableName),
                         "ORDER BY", quote("id_" + tableName)};
    // вывод из базы в модель
    model->setQuery(vecToStr(args));
}*/

void Database::exec(const QString &str)
{
    if (!query.exec(str)) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
}

void Database::execWithDisplay(const QString &str)
{
    if (!query.exec(str)) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else model->setQuery(query);
}

std::map<int32_t, QString> Database::getAttributesList(const QString &str)
{
    Tokens args = {"SELECT", quote("id_" + str), "FROM", quote(str)};
    query.exec(vecToStr(args));
    std::map<int32_t, QString> map;
    while (query.next())
    {
        map[query.value(0).toInt()] = query.value(0).toString();
    }
    return map;
}
