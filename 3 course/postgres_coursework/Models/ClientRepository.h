#ifndef CLIENTREPOSITORY_H
#define CLIENTREPOSITORY_H

#include <QString>
#include "IRepository.h"

struct ClientModel
{
    int id;
    QString lastname;
    QString firstname;
    QString otchestvo;
    QString telephone;
    QString email;
};

class ClientRepository : public IRepository<ClientModel>
{
    Database& db;

    virtual bool read() const override
    { Q_UNIMPLEMENTED(); return false; }

    virtual bool update(const ClientModel&) override
    { Q_UNIMPLEMENTED(); return false; }

    virtual bool remove(int) override
    { Q_UNIMPLEMENTED(); return false; }

public:
    ClientRepository(Database& _db)
        : db(_db)
    {}
    virtual bool create(const ClientModel &data) override;
};

#endif // CLIENTREPOSITORY_H
