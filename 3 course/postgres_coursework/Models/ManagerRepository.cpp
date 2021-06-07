#include "ManagerRepository.h"
#include "StringTools.h"

using namespace StringTools;

QString ManagerRepository::getSelectQuery()
{
    const Tokens args = {"SELECT id_менеджер",
                         ", фамилия as", quote("Фамилия"),
                         ", имя as", quote("Имя"),
                         ", отчество as", quote("Отчество"),
                         ", телефон as", quote("Телефон"),
                         ", email as", quote("Электронная почта"),
                         "FROM менеджер"};
    return vecToStr(args);
}

void ManagerRepository::create(const ManagerModel &data)
{
    db.prepare("SELECT add_manager(:lastname, :firstname, :otchestvo, :telephone, :email, :password)");
    db.bindValue(":lastname", convertIfNull(data.lastname));
    db.bindValue(":firstname", convertIfNull(data.firstname));
    db.bindValue(":otchestvo", convertIfNull(data.otchestvo));
    db.bindValue(":telephone", convertIfNull(data.telephone));
    db.bindValue(":email", convertIfNull(data.email));
    db.bindValue(":password", data.password);

    db.exec();
    this->read();
}

void ManagerRepository::read()
{
    db.execWithDisplay(getSelectQuery() + " ORDER BY id_менеджер");
}

void ManagerRepository::update(const ManagerModel &data)
{
    if (data.id)
    {
        db.prepare("SELECT update_manager(:id, :lastname, :firstname, :otchestvo, :telephone, :email, :password)");
        db.bindValue(":id", data.id);
        db.bindValue(":lastname", convertIfNull(data.lastname));
        db.bindValue(":firstname", convertIfNull(data.firstname));
        db.bindValue(":otchestvo", convertIfNull(data.otchestvo));
        db.bindValue(":telephone", convertIfNull(data.telephone));
        db.bindValue(":email", convertIfNull(data.email));
        db.bindValue(":password", data.password);

        db.exec();
        this->read();
    }
}

void ManagerRepository::remove(int id)
{
    if (id)
    {
        db.prepare("SELECT delete_manager(:id)");
        db.bindValue(":id", id);

        db.exec();
        this->read();
    }
}

void ManagerRepository::search(const ManagerModel &data)
{
    Tokens searchOptions;
    QString query = getSelectQuery() + " WHERE ";
    if (!data.lastname.isEmpty())
        searchOptions.emplace_back("LOWER(фамилия) LIKE LOWER('%" + data.lastname + "%')");

    if (!data.firstname.isEmpty())
        searchOptions.emplace_back("LOWER(имя) LIKE LOWER('%" + data.firstname + "%')");

    if (!data.otchestvo.isEmpty())
        searchOptions.emplace_back("LOWER(отчество) LIKE LOWER('%" + data.otchestvo + "%')");

    if (!data.telephone.isEmpty())
        searchOptions.emplace_back("телефон LIKE '%" + data.telephone + "%'");

    if (!data.email.isEmpty())
        searchOptions.emplace_back("LOWER(email) LIKE LOWER('%" + data.email + "%')");

    size_t N = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < N-1; ++i)
        {
            query += searchOptions[i] + " AND ";
        }
        query += searchOptions[N-1];
        db.execWithDisplay(query);
    }
    else
    {
        this->read();
    }
}
