#include "CategoryRepository.h"
#include "StringTools.h"

using namespace StringTools;

QString CategoryRepository::getSelectQuery() const
{
    const Tokens args = {"SELECT id_категория, название as",
                         quote("Название категории"),
                         "FROM категория"};
    return vecToStr(args);
}

bool CategoryRepository::create(const CategoryModel &data)
{
    db.prepare("INSERT INTO категория (название) VALUES (:name)");
    db.bindValue(":name", convertIfNull(data.name));

    const bool result = db.exec();
    if (result) this->read();

    return result;
}

bool CategoryRepository::read() const
{
    return db.execWithDisplay(getSelectQuery() + " ORDER BY id_категория");
}

bool CategoryRepository::update(const CategoryModel &data)
{
    bool result = false;
    if (data.id)
    {
        db.prepare("UPDATE категория SET название = :name WHERE id_категория = :id");
        db.bindValue(":id", data.id);
        db.bindValue(":name", convertIfNull(data.name));

        result = db.exec();
        if (result) this->read();
    }
    return result;
}

bool CategoryRepository::remove(int id)
{
    bool result = false;
    if (id)
    {
        db.prepare("DELETE FROM категория WHERE id_категория = (:id)");
        db.bindValue(":id", id);

        result = db.exec();
        if (result) this->read();
    }
    return result;
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

std::map<int, QString> CategoryRepository::getAttributesList() const
{
    return db.getAttributesList("SELECT id_категория, название FROM категория");
}
