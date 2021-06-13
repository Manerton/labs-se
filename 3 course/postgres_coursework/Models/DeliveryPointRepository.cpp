#include "DeliveryPointRepository.h"
#include "StringTools.h"

using namespace StringTools;

QString DeliveryPointRepository::getSelectQuery() const
{
    const Tokens args = {"SELECT id_пункт_выдачи",
                         ", название as", quote("Название пункта"),
                         ", адрес as", quote("Адрес"),
                         ", площадь as", quote("Площадь (кв. м.)"),
                         ", array_to_string(доп_функции, ' | ') as", quote("Дополнительные функции"),
                         "FROM пункт_выдачи"};
    return vecToStr(args);
}

QVariant DeliveryPointRepository::getExtraFuncStr(const std::vector<QString> &arr) const
{
    // получаем строку с массивом
    if (!arr.empty())
    {
        QString extraFuncStr = "{";
        size_t N = arr.size();
        for (size_t i = 0; i < N-1; ++i)
        {
            extraFuncStr += quote(arr[i]) + ",";
        }
        extraFuncStr += quote(arr[N-1]) + "}";
        return extraFuncStr;
    }

    // возвращаем NULL значение
    return QVariant();
}

bool DeliveryPointRepository::create(const DeliveryPointModel &data)
{
    db.prepare("INSERT INTO пункт_выдачи (название, адрес, площадь, доп_функции) "
               "VALUES (:name, :address, :area, :extraFunc)");
    db.bindValue(":name", convertIfNull(data.name));
    db.bindValue(":address", convertIfNull(data.address));
    db.bindValue(":area", data.area);
    db.bindValue(":extraFunc", getExtraFuncStr(data.extraFunc));

    bool result = db.exec();
    if (result) this->read();
    return result;
}

bool DeliveryPointRepository::read() const
{
    return db.execWithDisplay(getSelectQuery() + " ORDER BY id_пункт_выдачи");
}

bool DeliveryPointRepository::update(const DeliveryPointModel &data)
{
    bool result = false;
    if (data.id)
    {
        db.prepare("UPDATE пункт_выдачи "
                   "SET название = :name, адрес = :address, площадь = :area, доп_функции = :extraFunc "
                   "WHERE id_пункт_выдачи = :id");
        db.bindValue(":id", data.id);
        db.bindValue(":name", convertIfNull(data.name));
        db.bindValue(":address", convertIfNull(data.address));
        db.bindValue(":area", data.area);
        db.bindValue(":extraFunc", getExtraFuncStr(data.extraFunc));

        result = db.exec();
        if (result) this->read();
    }
    return result;
}

bool DeliveryPointRepository::remove(int id)
{
    bool result = false;
    if (id)
    {
        db.prepare("DELETE FROM пункт_выдачи WHERE id_пункт_выдачи = (:id)");
        db.bindValue(":id", id);

        result = db.exec();
        if (result) this->read();
    }
    return result;
}

void DeliveryPointRepository::search(const DeliveryPointModel &data)
{
    Tokens searchOptions;
    QString query = getSelectQuery() + " WHERE ";
    if (!data.name.isEmpty())
        searchOptions.emplace_back("название ILIKE '%" + data.name + "%'");

    if (!data.address.isEmpty())
        searchOptions.emplace_back("адрес ILIKE '%" + data.address + "%'");

    if (data.area > 0)
        searchOptions.emplace_back("cast(площадь as TEXT) ILIKE '%" + QString::number(data.area) + "%'");

    if (!data.extraFunc.empty())
    {
        searchOptions.emplace_back("array_to_string(доп_функции, ',') ILIKE ALL ('{");
        const size_t count = data.extraFunc.size();
        for (size_t i = 0; i < count-1; ++i)
        {
            searchOptions.back() += quote("%" + data.extraFunc[i] + "%") + ",";
        }
        searchOptions.back() += quote("%" + data.extraFunc[count-1] + "%") + "}')";
    }

    const size_t N = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < N-1; ++i)
        {
            query += searchOptions[i] + " AND ";
        }
        query += searchOptions[N-1];
        db.execWithDisplay(query);
    }
    else this->read();
}

std::map<int, QString> DeliveryPointRepository::getAttributesList() const
{
    return db.getAttributesList("SELECT id_пункт_выдачи, (название || ' - ' || адрес) FROM пункт_выдачи");
}

QString DeliveryPointRepository::getExtraFuncForId(int id) const
{
    db.exec("SELECT array_to_string(доп_функции, ' | ') FROM пункт_выдачи WHERE id_пункт_выдачи = " + QString::number(id));
    QString extraFunctions;
    extraFunctions = db.getFirstValue(0).toString();
    return extraFunctions;
}
