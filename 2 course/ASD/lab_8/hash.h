#ifndef HASH_H
#define HASH_H
#include <cstdint>
#include <tuple>

const uint32_t mask = 255; // 255 это для выделение 8 бит, 1023 для выделения 10 бит

class hash
{
public:
    using key_t   = uint64_t;
    using value_t = std::tuple<
    uint8_t,   // Family_member_count
    uint16_t,   // Year
    uint8_t,    // Month
    float,      // Area_occupied
    uint8_t,    // Rooms_count
    float       // Area_required
    >; // -- квартирант/очередник -- //
    using data_t  = std::tuple<key_t, value_t>;
    enum hashFunc : uint8_t
    {
        divideHash = 0,
        gornerDivide = 1,
        gornerHighlight = 2
    };
private:
    struct node_t
    {
        data_t element;
        key_t& key() {return std::get<0>(element);}
        value_t& value() {return std::get<1>(element);}
        // инициализация элементом
        node_t(const data_t &tmp) : element(tmp) {}
        // инициализация ключом без данных
        node_t(const key_t &key) : element({key,value_t()}) {}
        node_t() = default;
    };
// поля
    // размер таблицы (257 или 1024)
    static constexpr uint16_t table_size = 257;
    // иксы для схем Горнера
    static constexpr uint16_t x_gorner_1 = 23;
    static constexpr uint16_t x_gorner_2 = 256;
    // элемент с ключом EMPTYELEM означает пустой элемент таблицы
    static constexpr uint16_t EMPTYELEM = table_size+1;
    size_t _count = 0;
    node_t* arr = nullptr; // дин. массив с node_t
    // какая хеш-функция используется в данном хеш-таблице
    hashFunc currentHashFunc;
// функции
    size_t calculateDivideHashFunc(const key_t &key); // подсчет значения хэш функции
    size_t calculateGornerDivideHashFunc(const key_t &key); // подсчет значения хэш функции
    size_t calculateGornerHighlightHashFunc(const key_t &key); // подсчет значения хэш функции
    size_t get_gorner_result(uint64_t key, uint16_t x);
    node_t* find_pointer(const key_t &key);
public:
    hash();
    ~hash();
    hash(const hashFunc _hashFunc);
    hash(const hash& d);
    hash& operator=(const hash &other);
    hash(const data_t& d);
    hash(const std::initializer_list<data_t> &t);
// Количества ------------------
    bool empty() const noexcept;
    size_t count() const noexcept;              // -- количество элементов --
    // вызвать текущую хеш функцию
    size_t callCurrentHashFunc(const key_t &key);
// Получение значений
    static key_t get_key(const data_t &elem) noexcept;
    static value_t get_value(const data_t &elem) noexcept;
// Поиск -----------------------
    data_t find(const key_t& key) ;
// Модификаторы контейнера --
// Вставить --
    bool insert(const key_t& key, const value_t& v);
    bool insert(const data_t& d);
// Заменить --
    bool replace(const key_t& key, const value_t& v);
// Удалить --
    bool erase (const key_t& key);              // удалить указанный
    void clear ();                              // удалить все
// обмен --
    void swap (hash &t) noexcept;        // обменять с заданным списком
// итераторы для Std алгоритмов
    node_t* begin() { return arr; };
    node_t* end() { return arr+table_size; };
    node_t* begin() const { return arr; };
    node_t* end() const { return arr+table_size; };
};

// вывод элемента в строку
std::string elemToString(const hash::data_t &element);
#endif // HASH_H
