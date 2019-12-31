#include "model.h"
#include <sstream>

using namespace std;

void Model::CheckSaleProcent(double proc)
{
    if (proc < 0) throw negative_sale_proc_exception();
}

std::string Model::toString() const noexcept
{
    stringstream ss;
    ss << model_name << " " << cpu_type << " " << cpu_frequency << " " << ram_size << " " << hdd_size;
    ss << " " << videomemory_size << " " << Money::toString() << " " << count_in_stock << " " << sale_proc;
    return ss.str();
}

Money Model::get_price() const noexcept
{
    Money t(*this);
    return t;
}

void Model::set_price(const Money &price) noexcept
{
    set_rubles(price.get_rubles());
    set_kopecks(price.get_kopecks());
}

Money Model::get_price_for_N_models(uint32_t N) noexcept
{
    Money t = get_price();
    t *= N;
    return t;
}

Money Model::calculate_sale() noexcept
{
    Money t = get_price();
    t *= sale_proc;
    return t;
}

Money Model::calculate_price_with_sale() noexcept
{
    Money t = get_price();
    t -= calculate_sale();
    return t;
}

Model &Model::operator+=(uint16_t b) noexcept
{
    count_in_stock += b;
    return *this;
}

Model &Model::operator-=(uint16_t b) noexcept
{
    count_in_stock -= b;
    return *this;
}

Model operator+(Model &a, uint16_t &b) noexcept
{
    Model t = a;
    t += b;
    return t;
}

Model operator-(Model &a, uint16_t &b) noexcept
{
    Model t = a;
    t -= b;
    return t;
}

bool operator==(const Model &a, const Model &b)
{
    return (a.get_price() == b.get_price());
}
bool operator!=(const Model &a, const Model &b)
{
    return !(a == b);
}
bool operator<(const Model &a, const Model &b)
{
    return (a.get_price() < b.get_price());
}
bool operator>=(const Model &a, const Model &b)
{
    return !(a<b);
}

bool operator>(const Model &a, const Model &b)
{
    return (b < a);
}

bool operator<=(const Model &a, const Model &b)
{
    return !(b < a);
}

ostream& operator<<(ostream &t, const Model &r)
{
    istringstream ss(r.toString()); // -- делаем stringstream из строки toString -- //
    ostringstream out;  // -- для выходной строки -- //
    string temp;
    ss >> temp;
    out << "Модель компьютера: " << temp << endl;
    ss >> temp;
    out << "Тип процессора: " << temp << endl;
    ss >> temp;
    out << "Частота процессора: " << temp << endl;
    ss >> temp;
    out << "Объем памяти: " << temp << endl;
    ss >> temp;
    out << "Объем жесткого диска: " << temp << endl;
    ss >> temp;
    out << "Объем памяти видеокарты: " << temp << endl;
    ss >> temp;
    out << "Цена компьютера: " << temp << endl;
    ss >> temp;
    out << "Количество экземпляров в наличии: " << temp << endl;
    ss >> temp;
    out << "Процент скидки: " << temp << endl;
    return (t << out.str());
}

istream& operator>>(istream &t, Model &r)
{
    string model_name, cpu_type;
    uint16_t cpu_frequency;
    uint32_t ram_size, hdd_size, videomemory_size;
    Money price;
    uint16_t count_in_stock;
    double sale_proc;

    t >> model_name >> cpu_type >> cpu_frequency >> ram_size >> hdd_size >> videomemory_size;
    t >> price >> count_in_stock >> sale_proc;

    r = Model(model_name, cpu_type, cpu_frequency, ram_size, hdd_size,
              videomemory_size, price, count_in_stock, sale_proc);
    return t;
}
