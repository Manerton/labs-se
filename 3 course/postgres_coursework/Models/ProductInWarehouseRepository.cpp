#include "ProductInWarehouseRepository.h"
#include "StringTools.h"

using namespace StringTools;

bool ProductInWarehouseRepository::create(const ProductInWarehouseModel &data)
{
    db.prepare("SELECT add_product_to_warehouse (:id_product, :production_date, :count)");
    db.bindValue(":id_product", convertIfNull(data.id_product));
    db.bindValue(":production_date", data.production_date);
    db.bindValue(":count", data.count);

    bool result = db.exec();
    if (result) this->read();
    return result;
}

bool ProductInWarehouseRepository::read() const
{
    return db.execWithDisplay("SELECT * FROM товар_на_складе_v ORDER BY id_товар_на_складе");
}

bool ProductInWarehouseRepository::remove(int id)
{
    bool result = false;
    if (id)
    {
        db.prepare("DELETE FROM товар_на_складе WHERE id_товар_на_складе = (:id)");
        db.bindValue(":id", id);

        result = db.exec();
        if (result) this->read();
    }
    return result;
}

void ProductInWarehouseRepository::search(const ProductInWarehouseModel &data)
{
    Tokens searchOptions;
    QString query = "SELECT * FROM товар_на_складе_v WHERE ";

    if (data.id_product)
        searchOptions.emplace_back("Товар = (SELECT Производитель || ' ' || Наименование FROM товар_v WHERE id_товар = " + QString::number(data.id_product) + ")");

    if (!data.production_date.isNull())
        searchOptions.emplace_back("\"Дата изготовления\" = '" + data.production_date.toString(Qt::ISODate) + "'");

    if (data.count > 0)
        searchOptions.emplace_back("cast(Количество as text) ILIKE '%" + QString::number(data.count) + "%'");

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

