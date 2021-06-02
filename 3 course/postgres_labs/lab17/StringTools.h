#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <vector>
#include <QString>

namespace StringTools {
    using Tokens = std::vector<QString>;

    // строку взять в двойные кавычки
    static QString quote(const QString &str)
    {
        return ("\"" + str + "\"");
    }

    // в одинарные
    static QString singleQuote(const QString &str)
    {
        return ("'" + str + "'");
    }

    // слепить токены в строку
    static QString vecToStr(const Tokens &v, char delim = ' ')
    {
        QString result;
        for (auto &str : v)
        {
            result += delim + str;
        }
        return result;
    }

    // если строка пустая, то возвращаем строку NULL
    static QString convertIfNull(const QString &elem)
    {
        if (elem.trimmed().isEmpty()) return "NULL";
        return elem;
    }
}

#endif // STRINGTOOLS_H
