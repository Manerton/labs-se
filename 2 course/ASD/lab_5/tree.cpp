#include "tree.h"
#include "doctest.h"
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stack>

std::string elemToString(const Tree::data_t &element)
{
    std::stringstream ss;
    ss << Tree::get_key(element) << " "
       << int(std::get<0>(Tree::get_value(element))) << " "
       << std::get<1>(Tree::get_value(element)) << " "
       << int(std::get<2>(Tree::get_value(element))) << " "
       << std::get<3>(Tree::get_value(element)) << " "
       << int(std::get<4>(Tree::get_value(element))) << " "
       << std::get<5>(Tree::get_value(element));
    return ss.str();
}

void Tree::erase_all(Tree::node_t *&node)
{
    if(node != nullptr)				// если узел дерева существует
    {                               // функция вызовет сама себя
        erase_all(node->left);	// сначала для левого потомка,
        erase_all(node->right);  // после для правого потомка

        // освобождаем память и обнуляем указатели
        delete node;
        node = nullptr;
    }
}

void Tree::add_node(const Tree::key_t &key, const Tree::value_t &v, node_t *&node)
{
    if(node == nullptr)   // если корень или лист
    {
        node = new node_t(data_t{key,v});
    }
    else
    {
        if(key < get_key(node->element))    // если ключ добавляемого элемента меньше данного узла
        {
            add_node(key, v, node->left);     // то идем влево
        }
        else
        {
            add_node(key, v, node->right);    // иначе вправо
        }
    }
}

Tree::~Tree()
{
    clear();
}

Tree::Tree(const Tree::data_t &d) noexcept
{
    insert(d);
}

Tree::Tree(const std::initializer_list<Tree::data_t> &t)
{
    for (const auto &d : t)
    {
        insert(d);
    }
}

Tree::Tree(const Tree &other)
{
    PreOrderCopy(other.root);
}

Tree::Tree(Tree &&other) : root(other.root), _count(other._count)
{
    other._count = 0;
    other.root = nullptr;
}

Tree &Tree::operator=(Tree &&other)
{
    if (this != &other)
    {
        std::swap(_count,other._count);
        std::swap(root,other.root);
    }
    return *this;
}

Tree &Tree::operator=(const Tree &other)
{
    if (this != &other)
    {
        Tree temp(other);
        std::swap(_count,temp._count);
        std::swap(root,temp.root);
    }
    return *this;
}

Tree::key_t Tree::get_key(const Tree::data_t &elem) noexcept
{
    return std::get<0>(elem);
}

Tree::value_t Tree::get_value(const Tree::data_t &elem) noexcept
{
    return std::get<1>(elem);
}

Tree::data_t Tree::get_root_data()
{
    return root->element;
}

Tree::key_t Tree::get_root_key()
{
    return get_key(root->element);
}

bool Tree::empty() const noexcept
{
    return (root == nullptr);
}

size_t Tree::count() const noexcept
{
    return _count;
}

size_t Tree::height() const noexcept
{
    return find_height(root)-1;
}

Tree::data_t Tree::find(const Tree::key_t &key)
{
    if (empty())
    {
        throw std::length_error("Дерево пустое, поиск невозможен.");
    }
    return findNode(key, root)->element;
}

Tree::data_t Tree::find_min()
{
    return find_min_node(root)->element;
}

Tree::data_t Tree::find_max()
{
    return find_max_node(root)->element;
}

bool Tree::insert(const Tree::key_t &key, const Tree::value_t &v)
{
    if (check_for_unique_key(key))
    {
        add_node(key,v,root);
        _count++;
        return true;
    }
    return false;
}

bool Tree::insert(const Tree::data_t &d)
{
    auto [key,v] = d;
    return insert(key,v);
}

bool Tree::replace(const Tree::key_t &key, const Tree::value_t &v)
{
    if (!empty())
    {
        auto toReplace = findNode(key, root);
        toReplace->element = {key,v};
        return true;
    }
    return false;
}

bool Tree::erase(const Tree::key_t &key)
{
    auto parentToErase = findParentNode(key,root);
    if (parentToErase != nullptr)
    {
        node_t* toErase = nullptr;
        if (parentToErase->left && get_key(parentToErase->left->element) == key)
        {
            toErase = parentToErase->left;
        }
        else if (parentToErase->right && get_key(parentToErase->right->element) == key)
        {
            toErase = parentToErase->right;
        }
        removeNode(parentToErase,toErase);
        return true;
    }
    return false;
}

void Tree::clear()
{
    erase_all(root);
    _count = 0;
}

void Tree::swap(Tree &t) noexcept
{
    std::swap(root, t.root);
    std::swap(_count, t._count);
}

void Tree::PreOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoPreOrderRecurse(toWrite, root);
    toWrite.close();
}

void Tree::InOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoInOrderRecurse(toWrite, root);
    toWrite.close();
}

void Tree::PostOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoPostOrderRecurse(toWrite, root);
    toWrite.close();
}

void Tree::PreOrderIterative(const std::string &filename) const noexcept
{
    if (!empty())
    {
        std::ofstream toWrite(filename);
        if (toWrite.is_open()) // если удалось начать запись
        {
            std::stack<node_t*> stack; // для записи элементов
            node_t* current = root;
            while (current != nullptr || !stack.empty()) // пока не дойдем до последнего элемента или стек не пуст
            {
                while (current != nullptr) // ищем последний элемент
                {
                    stack.push(current);
                    toWrite << elemToString(current->element) << std::endl;
                    current = current->left;
                }
                current = stack.top()->right;
                stack.pop();
            }
        }
        toWrite.close();
    }
}

void Tree::InOrderIterative(const std::string &filename) const noexcept
{
    if (!empty())
    {
        std::ofstream toWrite(filename);
        if (toWrite.is_open()) // если удалось начать запись
        {
            std::stack<node_t*> stack; // для записи элементов
            node_t* current = root;
            while (current != nullptr || !stack.empty()) // пока не дойдем до последнего элемента или стек не пуст
            {
                while (current != nullptr) // ищем последний элемент
                {
                    stack.push(current);
                    current = current->left;
                }
                toWrite << elemToString(stack.top()->element) << std::endl;
                current = stack.top()->right;
                stack.pop();
            }
        }
        toWrite.close();
    }
}

void Tree::PostOrderIterative(const std::string &filename) const noexcept
{
    if (!empty())
    {
        std::ofstream toWrite(filename);
        if (toWrite.is_open())
        {
            std::stack<node_t *> stack, stackR; // для записи элементов
            node_t* current = root;
            stack.push(current);
            while (!stack.empty()) // пока не дойдем до последнего элемента или стек не пуст
            {
                current = stack.top();
                stack.pop();
                stackR.push(current);
                if (current->left)  stack.push(current->left);
                if (current->right) stack.push(current->right);
            }
            while(!stackR.empty()) // стек, который заполнялся с правого элемента
            {
                toWrite << elemToString(stackR.top()->element) << std::endl; // запись данных в файл
                stackR.pop();
            }
        }
    }
}

void Tree::concat(const Tree &b)
{
    if (root != b.root)
    {
        PreOrderCopy(b.root);
    }
}

// прямой обход (сверху вниз)
void Tree::DoPreOrderRecurse(std::ofstream &write, Tree::node_t *node) const
{
    if (node != nullptr)
    {
        write << elemToString(node->element) << std::endl;
        DoPreOrderRecurse(write, node->left);
        DoPreOrderRecurse(write, node->right);
    }
}

void Tree::DoInOrderRecurse(std::ofstream &write, Tree::node_t *node) const
{
    if (node != nullptr)
    {
        DoInOrderRecurse(write, node->left);
        write << elemToString(node->element) << std::endl;
        DoInOrderRecurse(write, node->right);
    }
}

void Tree::DoPostOrderRecurse(std::ofstream &write, Tree::node_t *node) const
{
    if (node != nullptr)
    {
        DoPostOrderRecurse(write, node->left);
        DoPostOrderRecurse(write, node->right);
        write << elemToString(node->element) << std::endl;
    }
}

Tree::node_t *Tree::findNode(const Tree::key_t &key, Tree::node_t *node)
{
    if (node != nullptr)
    {
        if(key == get_key(node->element))   // если нашли ключ
        {
            return node;
        }
        else if(key < get_key(node->element))
        {
            return findNode(key, node->left);
        }
        else
        {
            return findNode(key, node->right);
        }
    }
    else
    {
        return root;
    }
}

Tree::node_t *Tree::findParentNode(const Tree::key_t &key, Tree::node_t *node)
{
    if (node != nullptr)
    {
        auto l = node->left;
        auto r = node->right;
        if (l || r) // если есть хоть один потомок
        {
            // проверим ключ у левого дочернего узла
            if((l) && (key == get_key(l->element)))
            {
                return node; // если нашли ключ
            }
            // проверим ключ у правого дочернего узла
            else if ((r) && (key == get_key(r->element)))
            {
                return node;
            }
            else if(key < get_key(node->element))
            {
                return findParentNode(key, node->left);
            }
            else
            {
                return findParentNode(key, node->right);
            }
        }
        else return root;   // дошли до листа, он явно не тот узел, что мы искали
    }
    else return root;   // ничего не нашли
}

bool Tree::check_for_unique_key(const Tree::key_t &key)
{
    if (!empty()) // если корень не пустой, то ключ надо проверить
    {
        // если был найден корень (который не равен ключу), то
        // значит ключ уникальный и его нет в дереве
        // иначе ключ неуникальный
        auto res = find(key);
        return ((get_root_key() != key) && (get_key(res) == get_root_key()));
    }
    return true;
}

size_t Tree::find_height(Tree::node_t *node) const noexcept
{
    return node == nullptr ? 0 : (1 + std::max(find_height(node->left), find_height(node->right)));
}

void Tree::PreOrderCopy(Tree::node_t *node)
{
    if (node != nullptr)
    {
        insert(node->element);
        PreOrderCopy(node->left);
        PreOrderCopy(node->right);
    }
}

Tree::node_t *Tree::find_min_node(Tree::node_t *node)
{
    if (empty())
    {
        throw std::length_error("Дерево пустое, поиск невозможен.");
    }

    // минимальный элмент в двоичном дереве поиска в крайне левом узле

    node_t* current = node;
    while (current->left)  current = current->left; // сдиваегся влево, пока слева есть элемент
    return current; // вернем найденный элемент
}

Tree::node_t *Tree::find_max_node(Tree::node_t *node)
{
    if (empty())
    {
        throw std::length_error("Дерево пустое, поиск невозможен.");
    }

    // максимальный элмент в двоичном дереве поиска в крайне правом узле

    node_t* current = node;
    while (current->right)  current = current->right; // сдиваегся влево, пока слева есть элемент
    return current; // вернем найденный элемент
}

void Tree::removeNode(Tree::node_t *parent, node_t *&target)
{
    if (target)
    {
        auto l = target->left;
        auto r = target->right;
        if (l && r) // оба наследника есть, значит заменяем узел на максимум левого поддерева
        {
            auto localMax = find_max_node(l);
            auto localMaxkey = get_key(localMax->element);
            target->element = localMax->element;
            // и удаляем максимум левого поддерева, только нужно найти его род. узел тоже
            removeNode(findParentNode(localMaxkey,target),localMax);
            return;
        }
        else if (l) // есть только левый наследник
        {
            if (target == parent->left)
            {
                parent->left = target->left;
            } else
            {
                parent->right = target->left;
            }
        }
        else if (r) // есть только правый наследник
        {
            if (target == parent->right)
            {
                parent->right = target->right;
            } else
            {
                parent->left = target->right;
            }
        }
        else  // нет наследников
        {
            if (target == parent->left) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
        }
        delete target;
    }
}
