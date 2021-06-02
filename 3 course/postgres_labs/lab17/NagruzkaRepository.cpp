#include "NagruzkaRepository.h"
#include "StringTools.h"

using namespace StringTools;

QString NagruzkaRepository::getSelectQuery()
{
    const Tokens args = {"SELECT id_нагрузка",
                         ", id_преподаватель as", quote("Номер преподавателя"),
                         ", id_группа as", quote("Номер группы"),
                         ", количество_часов as", quote("Количество часов"),
                         ", предмет as", quote("Предмет"),
                         ", тип_занятия as", quote("Тип занятия"),
                         ", оплата as", quote("Оплата"),
                         "FROM нагрузка"};
    return vecToStr(args);
}

void NagruzkaRepository::read()
{
    dbContext.execWithDisplay(getSelectQuery() + " ORDER BY id_нагрузка");
}

void NagruzkaRepository::create(const NagruzkaModel& data)
{
    const Tokens args = {"INSERT INTO нагрузка (id_преподаватель, id_группа, количество_часов, предмет, тип_занятия, оплата) VALUES (",
                         convertIfNull(data.id_lecturer) + ",",
                         convertIfNull(data.id_group) + ",",
                         convertIfNull(data.hours) + ",",
                         singleQuote(convertIfNull(data.subject)) + ",",
                         singleQuote(convertIfNull(data.type_subject)) + ",",
                         convertIfNull(data.pay) + ")"};
    dbContext.exec(vecToStr(args));
    this->read();
}

void NagruzkaRepository::update(const NagruzkaModel& data)
{
    const Tokens args = {"UPDATE нагрузка SET",
                         " id_преподаватель = ", convertIfNull(data.id_lecturer),
                         ", id_группа = ", convertIfNull(data.id_group),
                         ", количество_часов = ", convertIfNull(data.hours),
                         ", предмет = ", singleQuote(convertIfNull(data.subject)),
                         ", тип_занятия = ", singleQuote(convertIfNull(data.type_subject)),
                         ", оплата = ", convertIfNull(data.pay),
                         " WHERE id_нагрузка = ", convertIfNull(data.id)};
    dbContext.exec(vecToStr(args));
    this->read();
}

void NagruzkaRepository::remove(const QString &id)
{
    const QString str = {"DELETE FROM нагрузка WHERE id_нагрузка = " + convertIfNull(id)};
    dbContext.exec(str);
    this->read();
}

void NagruzkaRepository::search(const NagruzkaModel& data)
{
    Tokens searchOptions;
    QString query = getSelectQuery() + " WHERE ";
    if (!data.id_lecturer.isEmpty())
    {
        searchOptions.emplace_back("id_преподаватель = " + data.id_lecturer);
    }
    if (!data.id_group.isEmpty())
    {
        searchOptions.emplace_back("id_группа = " + data.id_group);
    }
    if (!data.hours.isEmpty())
    {
        searchOptions.emplace_back("cast(количество_часов as TEXT) LIKE '%" + data.hours + "%'");
    }
    if (!data.subject.isEmpty())
    {
        searchOptions.emplace_back("предмет LIKE '%" + data.subject + "%'");
    }
    if (!data.type_subject.isEmpty())
    {
        searchOptions.emplace_back("тип_занятия LIKE '%" + data.type_subject + "%'");
    }
    if (!data.pay.isEmpty())
    {
        searchOptions.emplace_back("cast(оплата as TEXT) LIKE '%" + data.pay + "%'");
    }

    size_t N = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < N-1; ++i)
        {
            query += searchOptions[i] + " AND ";
        }
        query += searchOptions[N-1];
        dbContext.execWithDisplay(query);
    }
    else
    {
        this->read();
    }
}
