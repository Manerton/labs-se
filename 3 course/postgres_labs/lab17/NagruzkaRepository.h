#ifndef NAGRUZKAREPOSITORY_H
#define NAGRUZKAREPOSITORY_H

#include <cstdint>
#include <QString>
#include "Database.h"

class NagruzkaRepository
{
public:
    // строка таблицы "нагрузка"
    struct NagruzkaModel {
        QString id;
        QString id_lecturer;
        QString id_group;
        QString hours;
        QString subject;
        QString type_subject;
        QString pay;
    };
private:
    Database& dbContext;
    QString getSelectQuery();
public:
    explicit NagruzkaRepository(Database& _dbContext)
        : dbContext(_dbContext)
    {}

    void create(const NagruzkaModel& data);
    void read();
    void update(const NagruzkaModel& data);
    void remove(const QString& id);
    void search(const NagruzkaModel& criterias);
};

#endif // NAGRUZKAREPOSITORY_H
