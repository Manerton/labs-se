#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Database.h"

template <class T>
class IRepository
{
public:
    virtual void create(const T& data) = 0;
    virtual void read() const = 0;
    virtual void update(const T& data) = 0;
    virtual void remove(int id) = 0;
    virtual ~IRepository() {}
};

#endif // REPOSITORY_H
