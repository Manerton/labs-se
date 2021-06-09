#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <vector>
#include <QString>
#include <QVariant>

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

    // если строка пустая, то возвращаем NULL
    static QVariant convertIfNull(const QString &elem)
    {
        if (elem.trimmed().isEmpty()) return QVariant();
        return elem;
    }
    static QVariant convertIfNull(const int &elem)
    {
        if (!elem) return QVariant();
        return elem;
    }
}

#endif // STRINGTOOLS_H
