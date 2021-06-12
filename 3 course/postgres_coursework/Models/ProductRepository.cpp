#include "ProductRepository.h"
#include "StringTools.h"
#include <QJsonDocument>

using namespace StringTools;

void ProductRepository::create(const ProductModel &data)
{
    db.prepare("INSERT INTO товар (id_производитель, id_категория, наименование, стоимость, гарантийный_срок, характеристики) "
               "VALUES (:id_manufacturer, :id_category, :name, :cost, :warranty, :specs)");
    db.bindValue(":id_manufacturer", convertIfNull(data.id_manufacturer));
    db.bindValue(":id_category", convertIfNull(data.id_category));
    db.bindValue(":name", convertIfNull(data.name));
    db.bindValue(":cost", data.cost);
    db.bindValue(":warranty", data.warranty);

    QString jsonStr = QJsonDocument(data.specs).toJson(QJsonDocument::Compact);
    db.bindValue(":specs", jsonStr != "{}" ? jsonStr : QVariant());

    db.exec();
    this->read();
}

void ProductRepository::read() const
{
    db.execWithDisplay("SELECT * FROM товар_v ORDER BY id_товар");
}

void ProductRepository::readForBuyer() const
{
    db.execWithDisplay("SELECT * FROM каталог_товаров_v ORDER BY id_товар");
}

void ProductRepository::update(const ProductModel &data)
{
    if (data.id)
    {
        db.prepare("UPDATE товар "
                   "SET id_производитель = :id_manufacturer, id_категория = :id_category, наименование = :name, "
                   "стоимость = :cost, гарантийный_срок = :warranty, характеристики = :specs "
                   "WHERE id_товар = :id");
        db.bindValue(":id", data.id);
        db.bindValue(":id_manufacturer", convertIfNull(data.id_manufacturer));
        db.bindValue(":id_category", convertIfNull(data.id_category));
        db.bindValue(":name", convertIfNull(data.name));
        db.bindValue(":cost", data.cost);
        db.bindValue(":warranty", data.warranty);

        QString jsonStr = QJsonDocument(data.specs).toJson(QJsonDocument::Compact);
        db.bindValue(":specs", jsonStr != "{}" ? jsonStr : QVariant());

        db.exec();
        this->read();
    }
}

void ProductRepository::remove(int id)
{
    if (id)
    {
        db.prepare("DELETE FROM товар WHERE id_товар = (:id)");
        db.bindValue(":id", id);

        db.exec();
        this->read();
    }
}

bool ProductRepository::DoSearch(const ProductModel &data, const QString& _query)
{
    Tokens searchOptions;
    QString query = _query;

    if (data.id_manufacturer)
        searchOptions.emplace_back("Производитель = (SELECT название FROM производитель WHERE id_производитель = " + QString::number(data.id_manufacturer) + ")");

    if (data.id_category)
        searchOptions.emplace_back("Категория = (SELECT название FROM категория WHERE id_категория = " + QString::number(data.id_category) + ")");

    if (!data.name.isEmpty())
        searchOptions.emplace_back("Наименование ILIKE '%" + data.name + "%'");

    if (data.cost > 0)
        searchOptions.emplace_back("cast(Стоимость as text) ILIKE '%" + QString::number(data.cost) + "%'");

    if (data.warranty > 0)
        searchOptions.emplace_back("cast(\"Гарантийный срок\" as text) ILIKE '%" + QString::number(data.warranty) + "%'");

    const size_t N = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < N-1; ++i)
        {
            query += searchOptions[i] + " AND ";
        }
        query += searchOptions[N-1];
        db.execWithDisplay(query);
        return true;
    }
    return false;
}

void ProductRepository::search(const ProductModel &data)
{
    if (!DoSearch(data, "SELECT * FROM товар_v WHERE "))
    {
        this->read();
    }
}

void ProductRepository::searchForBuyer(const ProductModel &data)
{
    if (!DoSearch(data, "SELECT * FROM каталог_товаров_v WHERE "))
    {
        this->readForBuyer();
    }
}

std::map<int, QString> ProductRepository::getAttributesList() const
{
    return db.getAttributesList("SELECT id_товар, (Производитель || ' ' || Наименование) FROM товар_v");
}

std::map<int, ProductRepository::productNameWithPrice> ProductRepository::getAttributesListWithPrices() const
{
    db.exec("SELECT id_товар, (Производитель || ' ' || Наименование), Стоимость FROM товар_v");
    std::map<int, productNameWithPrice> map;
    while (db.next())
    {
        map[db.value(0).toInt()] = {db.value(1).toString(), db.value(2).toFloat()};
    }
    return map;
}

QJsonDocument ProductRepository::getJsonSpecs(int id) const
{
    db.exec("SELECT характеристики FROM товар WHERE id_товар = " + QString::number(id));
    db.first();
    return QJsonDocument::fromJson(db.value(0).toString().toUtf8());
}

int ProductRepository::getProductAllCount(const int id) const
{
    db.exec("SELECT Количество FROM каталог_товаров_v WHERE id_товар = " + QString::number(id));
    db.first();
    return db.value(0).toInt();
}
