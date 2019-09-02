#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

struct NOTEBOOK
{
    char model[21]; // наименование
    struct size_type // габаритные размеры
    {
        float x;
        float y;
        float z;
    } size;
    float w; // вес
    int price; // цена
    int cpu; // частота процессора в МГц
    int ram; // количество оперативной памяти в мб
};

#endif // STRUCT_H_INCLUDED
