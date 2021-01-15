#include "database.h"
#include <QDebug>
#include <cmath>
#include <QtSql/QSqlError>
std::shared_ptr<QSqlQueryModel> Database::getModel()
{
    return model;
}

void Database::select()
{
    // вывод из базы в модель
    model->setQuery(R"(SELECT * FROM "Нагрузка" ORDER BY "ID_Нагрузка")");
}

// вставка
void Database::insert(const Database::tableElem &elem, const QString &tableName)
{
    const double oplata = elem.hourCost * elem.hours;
    // подготавливаем команду INSERT
    query.prepare("INSERT INTO \"" + tableName + "\" VALUES (:ID, :eduYear, :prepod, :groupID, :hours, :lesson, :hourCost, :oplata)");
    bindValues(elem,query);
    query.bindValue(":oplata", convertIfNull(oplata));
    // если ошибка, то выводим ошибку
    if (!query.exec()) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    // иначе обновляем содержимое таблицы
    else this->select();
}

// модификация данных в таблице
void Database::update(const Database::tableElem &elem, const QString &tableName)
{
    const double oplata = elem.hourCost * elem.hours;
    query.prepare("UPDATE \"" + tableName + R"(" SET "Учебный год"=:eduYear,
    "ФИО преподавателя"=:prepod,"Номер группы"=:groupID, "Количество часов"=:hours,
    "Предмет"=:lesson, "Стоимость одного часа"=:hourCost, "Оплата"=:oplata WHERE "ID_Нагрузка"=:ID)");
    bindValues(elem,query);
    query.bindValue(":oplata", convertIfNull(oplata));
    if (!query.exec()) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->select();
}

void Database::erase(const Database::tableElem &elem, const QString &tableName)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM \"" + tableName + "\" WHERE \"ID_" + tableName + "\" = :ID");
    query.bindValue(":ID", convertIfNull(elem.ID));
    if (!query.exec()) QMessageBox::critical(nullptr,"Ошибка",query.lastError().text());
    else this->select();
}

void Database::search(const Database::tableElem &elem, const QString &tableName)
{
    std::vector<QString> searchOptions;
    QString zapros = "SELECT * FROM \"" + tableName + "\" WHERE ";
    const QVariant nullObject(QVariant::String);
    if (convertIfNull(elem.ID) != nullObject)
    {
        searchOptions.emplace_back(R"(cast("ID_Нагрузка" as TEXT) LIKE '%)" + QString::number(elem.ID) + "%'");
    }
    if (convertIfNull(elem.eduYear) != nullObject)
    {
        searchOptions.emplace_back(R"(cast("Учебный год" as TEXT) LIKE '%)" + QString::number(elem.eduYear) + "%'");
        query.bindValue(":eduYear", elem.eduYear);
    }
    if (convertIfNull(elem.prepod) != nullObject)
    {
        searchOptions.emplace_back(R"("ФИО преподавателя" LIKE '%)" + elem.prepod + "%'");
        query.bindValue(":prepod", elem.prepod);
    }
    if (convertIfNull(elem.groupID) != nullObject)
    {
        searchOptions.emplace_back(R"(cast("Номер группы" as TEXT) LIKE '%)" + QString::number(elem.groupID) + "%'");
        query.bindValue(":groupID", elem.groupID);
    }
    if (convertIfNull(elem.hours) != nullObject)
    {
        searchOptions.emplace_back(R"(cast("Количество часов" as TEXT) LIKE '%)" + QString::number(elem.hours) + "%'");
        query.bindValue(":hours", elem.hours);
    }
    if (convertIfNull(elem.lesson) != nullObject)
    {
        searchOptions.emplace_back(R"("Предмет" LIKE '%)"+ elem.lesson + "%'");
        query.bindValue(":lesson", elem.lesson);
    }
    if (convertIfNull(elem.hourCost) != nullObject)
    {
        searchOptions.emplace_back(R"(cast("Стоимость одного часа" as TEXT) LIKE '%)"+ QString::number(elem.hourCost) + "%'");
        query.bindValue(":hourCost", elem.hourCost);
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
        this->select();
    }
}

void Database::bindValues(const Database::tableElem &elem, QSqlQuery &query)
{
    query.bindValue(":ID", convertIfNull(elem.ID));
    query.bindValue(":eduYear", convertIfNull(elem.eduYear));
    query.bindValue(":prepod", convertIfNull(elem.prepod));
    query.bindValue(":groupID", convertIfNull(elem.groupID));
    query.bindValue(":hours", convertIfNull(elem.hours));
    query.bindValue(":lesson", convertIfNull(elem.lesson));
    query.bindValue(":hourCost", convertIfNull(elem.hourCost));
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

template <typename T>
QVariant Database::convertIfNull(const T &elem)
{
    if constexpr (std::is_same_v<T,QString>)
    {
        if (elem.trimmed().isEmpty()) return QVariant(QVariant::String);
    }
    else if constexpr (std::is_arithmetic_v<T>)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            if (fabs(elem - 0) <= std::numeric_limits<T>::epsilon()) return QVariant(QVariant::String);
        }
        else if (elem == 0) return QVariant(QVariant::String);
    }
    return QVariant(elem);
}
