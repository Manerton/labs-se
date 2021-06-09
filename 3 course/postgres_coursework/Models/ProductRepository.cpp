#include "ProductRepository.h"
#include "StringTools.h"

using namespace StringTools;

QString ProductRepository::getSelectQuery() const
{

}

void ProductRepository::create(const ProductModel &data)
{
    db.prepare("INSERT INTO товар (id_производитель, id_категория, наименование, стоимость, гарантийный_срок, характеристики) "
               "VALUES (:id_manufacturer, :id_category, :name, :cost, :warranty, :specs)");
    db.bindValue(":id_manufacturer", data.id_manufacturer);
    db.bindValue(":id_category", data.id_category);
    db.bindValue(":id_name", data.name);
    db.bindValue(":cost", data.cost);
    db.bindValue(":warranty", data.warranty);
    db.bindValue(":specs", data.specs);

    db.exec();
    this->read();
}

void ProductRepository::read() const
{
    db.execWithDisplay("SELECT * FROM товар_v ORDER BY id_товар");
}

void ProductRepository::update(const ProductModel &data)
{

}

void ProductRepository::remove(int id)
{

}

void ProductRepository::search(const ProductModel &data)
{

}
