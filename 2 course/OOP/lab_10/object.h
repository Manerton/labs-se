#ifndef OBJECT_H
#define OBJECT_H
#include <cstdint>

class Object
{
    static size_t count;        // количество элементов
public:
    Object();                       // увеличение кол-ва элементов
    virtual ~Object();                      // уменьшение колва элементов
    static size_t Count() noexcept;    // выдача кол-ва элементов
};

#endif // OBJECT_H
