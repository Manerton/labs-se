#ifndef CATEGORYREPOSITORY_H
#define CATEGORYREPOSITORY_H

#include <QString>
#include "IRepository.h"

struct CategoryModel
{
    int id;
    QString name;
};

class CategoryRepository : public IRepository<CategoryModel>
{
    Database& db;

    QString getSelectQuery();
public:
    CategoryRepository(Database& _db)
        : db(_db)
    {}
    virtual void create(const CategoryModel &data) override;
    virtual void read() override;
    virtual void update(const CategoryModel &data) override;
    virtual void remove(int id) override;
    void search(const CategoryModel &data);
};

#endif // CATEGORYREPOSITORY_H
