#include "ManufacturerRepository.h"
#include "StringTools.h"

using namespace StringTools;

QString ManufacturerRepository::getSelectQuery() const
{
    const Tokens args = {"SELECT id_производитель",
                         ", название as", quote("Название производителя"),
                         ", страна as", quote("Страна производителя"),
                         "FROM производитель"};
    return vecToStr(args);
}

bool ManufacturerRepository::create(const ManufacturerModel &data)
{
    db.prepare("INSERT INTO производитель (название, страна) VALUES (:name, :country)");
    db.bindValue(":name", convertIfNull(data.name));
    db.bindValue(":country", convertIfNull(data.country));

    bool result = db.exec();
    if (result) this->read();
    return result;
}

bool ManufacturerRepository::read() const
{
    return db.execWithDisplay(getSelectQuery() + " ORDER BY id_производитель");
}

bool ManufacturerRepository::update(const ManufacturerModel &data)
{
    bool result = false;
    if (data.id)
    {
        db.prepare("UPDATE производитель "
                   "SET название = :name, страна = :country "
                   "WHERE id_производитель = :id");
        db.bindValue(":id", data.id);
        db.bindValue(":name", convertIfNull(data.name));
        db.bindValue(":country", convertIfNull(data.country));

        result = db.exec();
        if (result) this->read();
    }
    return result;
}

bool ManufacturerRepository::remove(int id)
{
    bool result = false;
    if (id)
    {
        db.prepare("DELETE FROM производитель WHERE id_производитель = (:id)");
        db.bindValue(":id", id);

        result = db.exec();
        if (result) this->read();
    }
    return result;
}

void ManufacturerRepository::search(const ManufacturerModel &data)
{
    Tokens searchOptions;
    QString query = getSelectQuery() + " WHERE ";
    if (!data.name.isEmpty())
        searchOptions.emplace_back("LOWER(название) LIKE LOWER('%" + data.name + "%')");

    if (!data.country.isEmpty())
        searchOptions.emplace_back("LOWER(страна) LIKE LOWER('%" + data.country + "%')");

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

std::map<int, QString> ManufacturerRepository::getAttributesList() const
{
    return db.getAttributesList("SELECT id_производитель, название FROM производитель");
}
