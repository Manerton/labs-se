#include "CategoryRepository.h"
#include "StringTools.h"

using namespace StringTools;

QString CategoryRepository::getSelectQuery()
{
    const Tokens args = {"SELECT id_категория, название as",
                         quote("Название категории"),
                         "FROM категория"};
    return vecToStr(args);
}

void CategoryRepository::create(const CategoryModel &data)
{
    db.prepare("INSERT INTO категория (название) VALUES (:name)");
    db.bindValue(":name", convertIfNull(data.name));

    db.exec();
    this->read();
}

void CategoryRepository::read()
{
    db.execWithDisplay(getSelectQuery() + " ORDER BY id_категория");
}

void CategoryRepository::update(const CategoryModel &data)
{
    if (data.id)
    {
        db.prepare("UPDATE категория SET название = :name WHERE id_категория = :id");
        db.bindValue(":id", data.id);
        db.bindValue(":name", convertIfNull(data.name));

        db.exec();
        this->read();
    }
}

void CategoryRepository::remove(int id)
{
    if (id)
    {
        db.prepare("DELETE FROM категория WHERE id_категория = (:id)");
        db.bindValue(":id", id);

        db.exec();
        this->read();
    }
}

void CategoryRepository::search(const CategoryModel &data)
{
    Tokens searchOptions;
    QString query = getSelectQuery() + " WHERE ";
    if (!data.name.isEmpty())
    {
        query += ("LOWER(название) LIKE LOWER('%" + data.name + "%')");
        db.execWithDisplay(query);
    }
    else this->read();
}
