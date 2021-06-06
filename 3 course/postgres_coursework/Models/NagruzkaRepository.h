#ifndef NAGRUZKAREPOSITORY_H
#define NAGRUZKAREPOSITORY_H

#include "IRepository.h"
#include "QString"

struct NagruzkaModel
{
    int id;
    int id_lecturer;
    int id_group;
    uint16_t hours;
    QString subject;
    QString type_subject;
    float pay;

    NagruzkaModel(int id, int id_lecturer, int id_group, uint16_t hours,
                  const QString &subject, const QString &type_subject,
                  float pay)
        : id(id),
        id_lecturer(id_lecturer),
        id_group(id_group),
        hours(std::move(hours)),
        subject(subject),
        type_subject(type_subject),
        pay(pay)
    {}
};

class NagruzkaRepository : public IRepository<NagruzkaModel>
{
    Database& db;
public:
    NagruzkaRepository(Database& _db)
        : db(_db)
    {}
    virtual void create(const NagruzkaModel &data) override
    {
        db.prepare("INSERT INTO нагрузка (id_преподаватель, id_группа, количество_часов, предмет, тип_занятия, оплата) "
                          "VALUES (:id_lecturer, :id_group, :hours, :subject, :type_subject, :pay)");
        db.bindValue(":id_lecturer", data.id_lecturer);
        db.bindValue(":id_group", data.id_group);
        db.bindValue(":hours", data.hours);
        db.bindValue(":subject", data.subject);
        db.bindValue(":type_subject", data.type_subject);
        db.bindValue(":pay", data.pay);
        db.exec();

        this->read();
    }
    virtual void read() override
    {
        db.execWithDisplay("SELECT * FROM нагрузка ORDER BY id_нагрузка");
    }
    virtual void update(const NagruzkaModel &data) override
    {

    }
    virtual void remove(int id) override
    {

    }
};

#endif // NAGRUZKAREPOSITORY_H
