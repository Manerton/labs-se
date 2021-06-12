#include "ClientRepository.h"
#include "StringTools.h"

using namespace StringTools;

bool ClientRepository::create(const ClientModel &data)
{
    db.prepare("SELECT add_client(:lastname, :firstname, :otchestvo, :telephone, :email)");
    db.bindValue(":lastname", convertIfNull(data.lastname));
    db.bindValue(":firstname", convertIfNull(data.firstname));
    db.bindValue(":otchestvo", convertIfNull(data.otchestvo));
    db.bindValue(":telephone", convertIfNull(data.telephone));
    db.bindValue(":email", convertIfNull(data.email));

    return db.exec();
}
