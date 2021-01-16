#include "database.h"
#include <QDebug>
#include <cmath>
#include <map>
#include <QtSql/QSqlError>

// вернуть указатель на модель (для интерфейса)
std::shared_ptr<QSqlQueryModel> Database::getModel()
{
    return model;
}
// вывод таблицы tableName в модель
void Database::select(const QString &tableName)
{
    const Tokens args = {"SELECT", "*", "FROM", quotedStr(tableName), "ORDER BY", quotedStr("ID_" + tableName)};
    // вывод из базы в модель
    model->setQuery(vec_to_str(args));
}
// вывод последней таблицы (текущей) в модель
void Database::selectCurrent()
{
    // вывод из базы в модель
    model->setQuery(model->query().lastQuery());
}

QString Database::get_dbTableName(dbTableName dbTN)
{
    switch (dbTN)
    {
        case dbTableName::Nagruzka:
            return "Нагрузка";
        case dbTableName::Prepod:
            return "Преподаватель";
        case dbTableName::Group:
            return "Группа";
    }
    return "Неизвестно";
}

// для таблицы группа
// вставка
void Database::insert(const Database::group_tableElem &elem)
{
    // подготавливаем команду INSERT
    const Tokens args = {"INSERT INTO", quotedStr(get_dbTableName(dbTableName::Group)), "VALUES (",
                         (convertIfNull(elem.ID)), "," , singleQuotedStr(convertIfNull(elem.specialnost)), ",",
                         singleQuotedStr(convertIfNull(elem.otdelenie)), "," , convertIfNull(elem.count), ")"};
    // если ошибка, то выводим ошибку
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    // иначе обновляем содержимое таблицы
    else this->selectCurrent();
}

// модификация данных в таблице
void Database::update(const Database::group_tableElem &elem)
{
    const Tokens args = {"UPDATE", quotedStr(get_dbTableName(dbTableName::Group)), "SET",
                        quotedStr("Специальность"),"=",singleQuotedStr(elem.specialnost),",",
                        quotedStr("Отделение"),"=",singleQuotedStr(elem.otdelenie),",", quotedStr("Количество студентов"),"=",elem.count,
                        "WHERE", quotedStr("ID_Группа"),"=",elem.ID};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->selectCurrent();
}

void Database::erase(const Database::group_tableElem &elem)
{
    const Tokens args = {"DELETE FROM", quotedStr(get_dbTableName(dbTableName::Group)), "WHERE", quotedStr("ID_Группа"),"=",elem.ID};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->selectCurrent();
}

void Database::search(const Database::group_tableElem &elem)
{
    std::vector<QString> searchOptions;
    QString zapros = "SELECT * FROM" + quotedStr(get_dbTableName(dbTableName::Group)) + "WHERE ";
    if (!elem.ID.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("ID_Группа" as TEXT) LIKE '%)" + elem.ID + "%'");
    }
    if (!elem.count.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("Количество студентов" as TEXT) LIKE '%)" + elem.count + "%'");
    }
    if (!elem.otdelenie.isEmpty())
    {
        searchOptions.emplace_back(R"("Отделение" LIKE '%)" + elem.otdelenie + "%'");
    }
    if (!elem.specialnost.isEmpty())
    {
        searchOptions.emplace_back(R"("Специальность" LIKE '%)" + elem.specialnost + "%'");
    }

    size_t optionsSize = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < optionsSize-1; ++i)
        {
            zapros += searchOptions[i] + " AND ";
        }
        if (optionsSize >= 1)
        {
            zapros += searchOptions[optionsSize-1];
        }
        if (!query.exec(zapros)) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
        model->setQuery(query);
    }
    else
    {
        this->select(get_dbTableName(dbTableName::Group));
    }
}

// для таблицы преподаватель
void Database::insert(const Database::prepod_tableElem &elem)
{
    // подготавливаем команду INSERT
    const Tokens args = {"INSERT INTO", quotedStr(get_dbTableName(dbTableName::Prepod)), "VALUES (",
                         (convertIfNull(elem.ID)), "," ,singleQuotedStr(convertIfNull(elem.familiya)),",",
                         singleQuotedStr(convertIfNull(elem.name)),",", singleQuotedStr(convertIfNull(elem.otchestvo)),",",
                         singleQuotedStr(convertIfNull(elem.phone)),",", convertIfNull(elem.exp), ")"};
    // если ошибка, то выводим ошибку
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    // иначе обновляем содержимое таблицы
    else this->selectCurrent();
}
// модификация данных в таблице
void Database::update(const Database::prepod_tableElem &elem)
{
    const Tokens args = {"UPDATE", quotedStr(get_dbTableName(dbTableName::Prepod)), "SET",
                         quotedStr("Фамилия"),"=",singleQuotedStr(elem.familiya),",",
                         quotedStr("Имя"),"=",singleQuotedStr(elem.name),",",
                         quotedStr("Отчество"),"=",singleQuotedStr(elem.otchestvo),",",
                         quotedStr("Телефон"),"=",singleQuotedStr(elem.phone),",",
                         quotedStr("Стаж"),"=",elem.exp,
                         "WHERE", quotedStr("ID_Преподаватель"),"=",elem.ID};

    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->selectCurrent();
}

void Database::erase(const Database::prepod_tableElem &elem)
{
    const Tokens args = {"DELETE FROM", quotedStr(get_dbTableName(dbTableName::Prepod)), "WHERE", quotedStr("ID_Преподаватель"),"=",elem.ID};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->selectCurrent();
}

void Database::search(const Database::prepod_tableElem &elem)
{
    std::vector<QString> searchOptions;
    QString zapros = "SELECT * FROM" + quotedStr(get_dbTableName(dbTableName::Prepod)) + "WHERE ";
    if (!elem.ID.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("ID_Преподаватель" as TEXT) LIKE '%)" + elem.ID + "%'");
    }
    if (!elem.familiya.isEmpty())
    {
        searchOptions.emplace_back(R"("Фамилия" LIKE '%)" + elem.familiya + "%'");
    }
    if (!elem.name.isEmpty())
    {
        searchOptions.emplace_back(R"("Имя" LIKE '%)" + elem.name + "%'");
    }
    if (!elem.otchestvo.isEmpty())
    {
        searchOptions.emplace_back(R"("Отчество" LIKE '%)" + elem.otchestvo + "%'");
    }
    if (!elem.phone.isEmpty())
    {
        searchOptions.emplace_back(R"("Телефон" LIKE '%)" + elem.phone + "%'");
    }
    if (!elem.exp.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("Стаж" as TEXT) LIKE '%)" + elem.exp + "%'");
    }

    size_t optionsSize = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < optionsSize-1; ++i)
        {
            zapros += searchOptions[i] + " AND ";
        }
        if (optionsSize >= 1)
        {
            zapros += searchOptions[optionsSize-1];
        }
        if (!query.exec(zapros)) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
        model->setQuery(query);
    }
    else
    {
        this->select(get_dbTableName(dbTableName::Prepod));
    }
}

// для таблицы нагрузка
void Database::insert(const Database::nagruzka_tableElem &elem)
{
    // подготавливаем команду INSERT
    const Tokens args = {"INSERT INTO", quotedStr(get_dbTableName(dbTableName::Nagruzka)), "VALUES (",
                         (convertIfNull(elem.ID)), ",",
                         (convertIfNull(elem.prepodID)),",",
                         (convertIfNull(elem.groupID)),",",
                         (convertIfNull(elem.hours)),",",
                         singleQuotedStr(convertIfNull(elem.lesson)),",",
                         singleQuotedStr(convertIfNull(elem.typeLesson)),",",
                         convertIfNull(elem.pay), ")"};
    // если ошибка, то выводим ошибку
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    // иначе обновляем содержимое таблицы
    else this->selectCurrent();
}
// модификация данных в таблице
void Database::update(const Database::nagruzka_tableElem &elem)
{
    const Tokens args = {"UPDATE", quotedStr(get_dbTableName(dbTableName::Nagruzka)), "SET",
                         quotedStr("ID_Преподаватель"),"=",(elem.prepodID),",",
                         quotedStr("ID_Группа"),"=",(elem.groupID),",",
                         quotedStr("Количество часов"),"=",(elem.hours),",",
                         quotedStr("Предмет"),"=",singleQuotedStr(elem.lesson),",",
                         quotedStr("Тип занятия"),"=",singleQuotedStr(elem.typeLesson),",",
                         quotedStr("Оплата"),"=",elem.pay,
                         "WHERE", quotedStr("ID_Нагрузка"),"=",elem.ID};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->selectCurrent();
}

void Database::erase(const Database::nagruzka_tableElem &elem)
{
    const Tokens args = {"DELETE FROM", quotedStr(get_dbTableName(dbTableName::Nagruzka)), "WHERE", quotedStr("ID_Нагрузка"),"=",elem.ID};
    if (!query.exec(vec_to_str(args))) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->selectCurrent();
}

void Database::search(const Database::nagruzka_tableElem &elem)
{
    std::vector<QString> searchOptions;
    QString zapros = "SELECT * FROM" + quotedStr(get_dbTableName(dbTableName::Nagruzka)) + "WHERE ";
    if (!elem.ID.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("ID_Нагрузка" as TEXT) LIKE '%)" + elem.ID + "%'");
    }
    if (!elem.prepodID.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("ID_Преподаватель" as TEXT) LIKE '%)" + elem.prepodID + "%'");
    }
    if (!elem.groupID.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("ID_Группа" as TEXT) LIKE '%)" + elem.groupID + "%'");
    }
    if (!elem.hours.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("Количество часов" as TEXT) LIKE '%)" + elem.hours + "%'");
    }
    if (!elem.lesson.isEmpty())
    {
        searchOptions.emplace_back(R"("Предмет" LIKE '%)" + elem.lesson + "%'");
    }
    if (!elem.typeLesson.isEmpty())
    {
        searchOptions.emplace_back(R"("Тип занятия" LIKE '%)" + elem.typeLesson + "%'");
    }
    if (!elem.pay.isEmpty())
    {
        searchOptions.emplace_back(R"(cast("Оплата" as TEXT) LIKE '%)" + elem.pay + "%'");
    }

    size_t optionsSize = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < optionsSize-1; ++i)
        {
            zapros += searchOptions[i] + " AND ";
        }
        if (optionsSize >= 1)
        {
            zapros += searchOptions[optionsSize-1];
        }
        if (!query.exec(zapros)) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
        model->setQuery(query);
    }
    else
    {
        this->select(get_dbTableName(dbTableName::Nagruzka));
    }
}

QString Database::quotedStr(const QString &str) const
{
    return ("\"" + str + "\"");
}

QString Database::singleQuotedStr(const QString &str) const
{
    return ("'" + str + "'");
}

QString Database::vec_to_str(const Tokens &v) const
{
    QString result;
    for (auto &str : v)
    {
        result += " " + str;
    }
    return result;
}

std::vector<uint64_t> Database::getAttributesList(const QString &str)
{
    Tokens args = {"SELECT", quotedStr("ID_" + str), "FROM", quotedStr(str)};
    query.exec(vec_to_str(args));
    std::vector<uint64_t> v;
    while (query.next())
    {
        v.push_back(query.value(0).toULongLong());
    }
    return v;
}

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

QString Database::convertIfNull(const QString &elem)
{
    if (elem.trimmed().isEmpty()) return "NULL";
    else return elem;
}
