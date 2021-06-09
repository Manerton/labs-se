#ifndef ERRORS_H
#define ERRORS_H

#include <map>
#include <QString>
#include <QSqlError>

namespace Errors
{
    enum ErrCode: int {
        not_null_violation = 23502,
        check_violation = 23514,
        duplicate_object = 42710,
        reserved_name = 42939,
        unique_violation = 23505
    };

    static QString msg(QSqlError errorCode)
    {
        if (errorCode.type() == QSqlError::ConnectionError)
        {
            if (errorCode.text().indexOf("could not connect to server") != -1)
                return "Не удалось подключиться к базе данных!";
            if (errorCode.text().indexOf("no password supplied") != -1)
                return "Необходимо указать пароль для авторизации!";
            if (errorCode.text().indexOf("password authentication failed") != -1)
                return "Неправильный логин или пароль!";
            return "Непредвиденная ошибка, сообщите администратору следующее сообщение:\n" + errorCode.text();
        }
        else
        {
            const auto code = errorCode.nativeErrorCode().toInt();
            switch(code)
            {
                case ErrCode::not_null_violation:
                    return "Пожалуйста, заполните все обязательные поля!";
                case ErrCode::check_violation:
                {
                    if (errorCode.text().indexOf("менеджер_телефон_check") != -1)
                        return "Номер телефона должен состоять из 11 символов!";
                    if (errorCode.text().indexOf("пункт_выдачи_площадь_check") != -1)
                        return "Площадь помещения должна быть больше 0 кв. м.";
                    break;
                }
                case ErrCode::duplicate_object:
                {
                    if (errorCode.text().indexOf("role") != -1)
                        return "Данный пользователь уже зарегестрирован!";

                    break;
                }
                case ErrCode::unique_violation:
                    return "Нарушение требования уникальности значения в столбце таблицы!";
                case ErrCode::reserved_name:
                    return "Указанный логин занят другим пользователем!";
            }
            return "Непредвиденная ошибка, сообщите администратору следующий код: " + QString::number(code);
        }
    }
}

#endif // ERRORS_H

