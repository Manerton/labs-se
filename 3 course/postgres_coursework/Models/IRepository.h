#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Database.h"

template <class T>
class IRepository
{
public:
    virtual bool create(const T& data) = 0;
    virtual bool read() const = 0;
    virtual bool update(const T& data) = 0;
    virtual bool remove(int id) = 0;
    virtual ~IRepository() {}
};

#endif // REPOSITORY_H
