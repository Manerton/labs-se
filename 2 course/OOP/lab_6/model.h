#ifndef MODEL_H
#define MODEL_H
#include "money.h"
#include <string>


class Model : private Money
{
    std::string model_name;     // -- модель (марка) компьютера -- //
    std::string cpu_type;       // -- тип процессора -- //
    uint16_t cpu_frequency;     // -- частота процессора в мегагерцах-- //
    uint32_t ram_size;          // -- объем памяти озу в мегабайтах -- //
    uint32_t hdd_size;          // -- объем жесткого диска -- //
    uint32_t videomemory_size;  // -- объем памяти видеокарты -- //
    // -- цена: рубли и копейки передались по наследству от Money -- //
    uint16_t count_in_stock;    // -- количество экземпляров в наличии -- //
    double sale_proc;           // -- процент скидки -- //

    void CheckSaleProcent(double proc); // -- процент скидки должен быть > 0 -- //
public:
    Model(const std::string &_model_name = "default",
          const std::string &_cpu_type = "x64",
          uint16_t _cpu_frequency = 3500,
          uint32_t _ram_size = 8192,
          uint32_t _hdd_size = 122880,
          uint32_t _videomemory_size = 1024,
          const Money &_price = Money(),
          uint16_t _count_in_stock = 1,
          double _sale_proc = 0.25)
        : Money(_price),
          model_name(_model_name),
          cpu_type(_cpu_type),
          cpu_frequency(_cpu_frequency),
          ram_size(_ram_size),
          hdd_size(_hdd_size),
          videomemory_size(_videomemory_size),
          count_in_stock(_count_in_stock),
          sale_proc(_sale_proc)
    { CheckSaleProcent(_sale_proc); }   // -- проверяем на корректность процент скидки -- //
    std::string toString() const noexcept; // -- перевод в строку для вывода -- //
    // -- методы связанные с ценами -- //
    Money get_price() const noexcept;               // -- получить цену -- //
    void set_price(const Money &price) noexcept;    // -- установить цену -- //
    Money get_price_for_N_models(uint32_t N) noexcept;   // -- вычислить стоимость любого кол-ва компьютеров -- //
    Money calculate_sale() noexcept;
    Money calculate_price_with_sale() noexcept;
    // -- операции-методы присваивания -- //
    Model& operator+=(uint16_t b) noexcept;
    Model& operator-=(uint16_t b) noexcept;
     // -- дружеские операции -- //
    friend Model operator+(Model &a, uint16_t &b) noexcept;
    friend Model operator-(Model &a, uint16_t &b) noexcept;
    // -- операции сравнения и вывода -- //
    friend bool operator==(const Model &a, const Model &b);
    friend bool operator!=(const Model &a, const Model &b);
    friend bool operator<(const Model &a, const Model &b);
    friend bool operator>=(const Model &a, const Model &b);
    friend bool operator>(const Model &a, const Model &b);
    friend bool operator<=(const Model &a, const Model &b);
    // -- ввод / вывод -- //
    friend std::ostream& operator<<(std::ostream& t, const Model &r);
    friend std::istream& operator>>(std::istream& t, Model &r);
    // -- исключения -- //
    class negative_sale_proc_exception {};  // -- отрицательный процент скидки -- //
};

#endif // MODEL_H
