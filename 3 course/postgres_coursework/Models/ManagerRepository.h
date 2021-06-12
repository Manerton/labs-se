#ifndef MANAGERREPOSITORY_H
#define MANAGERREPOSITORY_H

#include <QString>
#include "IRepository.h"

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

    QString getSelectQuery() const;
public:
    ManagerRepository(Database& _db)
        : db(_db)
    {}
    virtual bool create(const ManagerModel &data) override;
    virtual bool read() const override;
    virtual bool update(const ManagerModel &data) override;
    virtual bool remove(int id) override;
    void search(const ManagerModel &data);
};
#endif // MANAGERREPOSITORY_H
