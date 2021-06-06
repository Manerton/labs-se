#ifndef MANAGERREPOSITORY_H
#define MANAGERREPOSITORY_H

#include "IRepository.h"
#include "QString"

struct ManagerModel
{
    int id;
    QString lastname;
    QString firstname;
    QString otchestvo;
    QString telephone;
    QString email;
    QString password;
};

class ManagerRepository : public IRepository<ManagerModel>
{
    Database& db;

    QString getSelectQuery();
public:
    ManagerRepository(Database& _db)
        : db(_db)
    {}
    virtual void create(const ManagerModel &data) override;
    virtual void read() override;
    virtual void update(const ManagerModel &data) override;
    virtual void remove(int id) override;
    void search(const ManagerModel &data);
};
#endif // MANAGERREPOSITORY_H