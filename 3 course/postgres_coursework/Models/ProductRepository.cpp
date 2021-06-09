#include "ProductRepository.h"
#include "StringTools.h"
#include <QJsonDocument>

using namespace StringTools;

QString ProductRepository::getSelectQuery() const
{

}

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

void ProductRepository::update(const ProductModel &data)
{
    if (data.id)
    {
        qDebug() << data.id;
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

void ProductRepository::search(const ProductModel &data)
{
    this->read();
}
