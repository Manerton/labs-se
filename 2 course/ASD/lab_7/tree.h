#ifndef TREE_H
#define TREE_H
#include <cstdint>
#include <tuple>
#include <exception>

class RBtree
{
public:
    // enum для node_t
    enum nodeColor : uint8_t
    {
        red = 0,
        black = 1
    };
    enum direction : uint8_t
    {
        left = 0,
        right = 1
    };
    // ID - идентификационный номер очередника
    using key_t   = uint64_t;
    // данные элемента контейнера
    using value_t = std::tuple<
        uint8_t,   // Family_member_count
        uint16_t,   // Year
        uint8_t,    // Month
        float,      // Area_occupied
        uint8_t,    // Rooms_count
        float       // Area_required
        >; // -- квартирант/очередник -- //
    // ключ (ID) + данные
    using data_t  = std::tuple<key_t, value_t>;
private:
    struct node_t
    {
    // данные + ключ узла
        data_t element;
    // красный или черный узел
        nodeColor color = red;
    // указатели на левого и правого потомка, и на родителя
        node_t *left    = nullptr;
        node_t *right   = nullptr;
        node_t *parent  = nullptr;
    // получить дядю этого узла
        node_t* uncle() const;
    // получить брата этого узла
        node_t* brother() const;
    // опустить узел ниже, и "y" теперь новый отец для этого узла
    // для функций поворотов
        void move_down(node_t* y);
    // выясняем, находится ли слева этот узел от родителя
        bool isLeftChild() const;
    // проверяем, есть ли у этого узла красный дочерний узел
        bool has_red_child() const;
    // конструктор
        node_t(const data_t &tmp, const nodeColor &_color = red) : element(tmp), color(_color) {};
        ~node_t()
        {
            left = right = nullptr;
        }
    };
    node_t *root = nullptr; // корень дерева
    size_t _count = 0;        // количество элементов в дереве

// полное рекурсивное удаление дерева
    void erase_all(node_t *&node);
// рекурсивное добавление узла в дерево
    void add_node(const key_t& key, const value_t& v);
// рекурсивные обходы
    void DoPreOrderRecurse(std::ofstream &write, node_t *node) const;
    void DoInOrderRecurse(std::ofstream &write, node_t *node) const;
    void DoPostOrderRecurse(std::ofstream &write, node_t *node) const;
// рекурсивный поиск нода
    node_t* findNode(const key_t& key, node_t *node);
// проверка на уникальный ключ
    bool check_for_unique_key(const key_t& key);
// для функции копирования / объединения двух деревьев
    void PreOrderCopy(node_t *node);
// поиск минимума и максимума
    node_t* find_min_node(node_t* node);
    node_t* find_max_node(node_t* node);
// функция, используемая при удалении узла
    void transplant(node_t* u, node_t* v);
// рекурсивное удаление нода
    void removeNode(node_t *p);
// восстановление свойств красно-черного дерева после вставки элемента
    void fixAfterInsert(node_t* node);
// восстановление свойств после удаления элемента
    void fixAfterDelete(node_t* node);
// поиск высоты
    int8_t find_height(RBtree::node_t *node) const noexcept;
public:
    RBtree() = default;
    virtual ~RBtree(void);
    RBtree(const data_t& d) noexcept;
    RBtree(const std::initializer_list<data_t> &t);
    RBtree(const RBtree& other);
    RBtree(RBtree&& other);                // -- конструктор переноса --
    RBtree& operator=(RBtree&& other);     // -- операция перемещания --
    RBtree& operator=(const RBtree& other);
// Получение значений
    static key_t get_key(const data_t &elem) noexcept;
    static value_t get_value(const data_t &elem) noexcept;
    data_t get_root_data();
    key_t get_root_key();
// Количества ------------------
    bool empty() const noexcept;
    size_t count() const noexcept;              // -- количенство элементов --
    int8_t height(node_t* node) const noexcept;             // -- высота узла поддерева --
    int8_t height() const noexcept;             // -- высота дерева --
// Повороты ---------------------
    void leftRotate(node_t* x) noexcept;
    void rightRotate(node_t* x) noexcept;
// Поиск ------------------------
    data_t find(const key_t& key);
    data_t find_min();
    data_t find_max();
// Модификаторы контейнера ------
// Вставить --
    bool insert(const key_t& key, const value_t& v);
    bool insert(const data_t& d);
// Заменить --
    bool replace(const key_t& key, const value_t& v);
// Удалить --
    bool erase (const key_t& key);              // удалить указанный
    void clear ();                              // удалить все
// обмен --
    void swap (RBtree &t) noexcept;        // обменять с заданным списком
// Обходы дерева ----------------
// обход дерева - рекурсивный
    void PreOrderRecurse(const std::string &filename) const noexcept;
    void InOrderRecurse(const std::string &filename) const noexcept;
    void PostOrderRecurse(const std::string &filename) const noexcept;
// обход дерева - итеративный
    void PreOrderIterative(const std::string &filename) const noexcept;
    void InOrderIterative(const std::string &filename) const noexcept;
    void PostOrderIterative(const std::string &filename) const noexcept;
// Объединение двух деревьев ----
    void concat(const RBtree &b);
};
// вывод элемента в строку
std::string elemToString(const RBtree::data_t &element);

#endif // TREE_H
