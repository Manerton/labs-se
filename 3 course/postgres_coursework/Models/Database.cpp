#include "Database.h"
#include <cmath>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <fstream>
#include "StringTools.h"
#include "Errors.h"

using namespace StringTools;

// подключение к БД
void Database::setConnection(std::string_view db_name, std::string_view login, std::string_view password)
{
    db.setDatabaseName(db_name.cbegin());

    std::ifstream settings("settings.ini");
    if (settings.is_open())
    {
        std::string line;
        std::getline(settings,line);
        QString qLine = QString::fromStdString(line);
        auto hostAndPort = qLine.split(":");
        db.setHostName(hostAndPort[0]);
        db.setPort(hostAndPort[1].toInt());
    }
    else
    {
        db.setHostName(hostname);
        db.setPort(port);
    }

    db.setUserName(login.cbegin());
    db.setPassword(password.cbegin());

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Ошибка", Errors::msg(db.lastError()));
        connected = false;
    }
    else connected = true;
}

bool Database::isConnected() const
{
    return connected;
}

bool Database::exec()
{
    const bool result = query.exec();
    if (!result)
        QMessageBox::critical(nullptr,"Ошибка", Errors::msg(query.lastError()));
    return result;
}

bool Database::exec(const QString &str)
{
    const bool result = query.exec(str);
    if (!result)
        QMessageBox::critical(nullptr,"Ошибка", Errors::msg(query.lastError()));
    return result;
}

bool Database::execWithDisplay()
{
    const bool result = query.exec();
    if (!result)
    {
        QMessageBox::critical(nullptr,"Ошибка", Errors::msg(query.lastError()));
    }
    else
    {
        model->setQuery(query);
    }
    return result;
}

bool Database::execWithDisplay(const QString &str)
{
    const bool result = query.exec(str);
    if (!result)
    {
        QMessageBox::critical(nullptr,"Ошибка", Errors::msg(query.lastError()));
    }
    else
    {
        model->setQuery(query);
    }
    return result;
}

bool Database::execWithDisplayToOtherModel(const QString &str, std::shared_ptr<QSqlQueryModel> _model)
{
    const bool result = query.exec(str);
    if (!result)
    {
        QMessageBox::critical(nullptr,"Ошибка", Errors::msg(query.lastError()));
    }
    else
    {
        _model->setQuery(query);
    }
    return result;
}

bool Database::first()
{
    return query.first();
}

bool Database::next()
{
    return query.next();
}

QVariant Database::getFirstValue(int i)
{
    this->first();
    return value(i);
}

bool Database::transaction()
{
    return db.transaction();
}

bool Database::commit()
{
    return db.commit();
}

bool Database::rollback()
{
    return db.rollback();
}

QVariant Database::value(int i)
{
    return query.value(i);
}

bool Database::prepare(const QString &str)
{
    bool result = query.prepare(str);
    if (!result)
        QMessageBox::critical(nullptr,"Ошибка", Errors::msg(query.lastError().nativeErrorCode()));
    return result;
}

void Database::bindValue(const QString &placeholder, const QVariant &val)
{
    query.bindValue(placeholder,val);
}

std::map<int, QString> Database::getAttributesList(const QString& str)
{
    this->exec(str);
    std::map<int, QString> map;
    while (query.next())
    {
        map[query.value(0).toInt()] = query.value(1).toString();
    }
    return map;
}
