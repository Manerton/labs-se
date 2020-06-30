#include "tree.h"
#include "doctest.h"
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stack>

std::string elemToString(const RBtree::data_t &element)
{
    std::stringstream ss;
    ss << RBtree::get_key(element) << " "
       << int(std::get<0>(RBtree::get_value(element))) << " "
       << std::get<1>(RBtree::get_value(element)) << " "
       << int(std::get<2>(RBtree::get_value(element))) << " "
       << std::get<3>(RBtree::get_value(element)) << " "
       << int(std::get<4>(RBtree::get_value(element))) << " "
       << std::get<5>(RBtree::get_value(element));
    return ss.str();
}

void RBtree::erase_all(RBtree::node_t *&node)
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

void RBtree::add_node(const RBtree::key_t &key, const RBtree::value_t &v)
{
    node_t* newNode = new node_t(data_t{key,v});
    if(root == nullptr)
    {
        newNode->color = black;
        root = newNode;
    }
    else
    {
        node_t* temp = findNode(key,root);
        newNode->parent = temp;
        if (key < get_key(temp->element))
        {
            temp->left = newNode;
        }
        else temp->right = newNode;
        fixAfterInsert(newNode);
    }
}

RBtree::~RBtree()
{
    clear();
}

RBtree::RBtree(const RBtree::data_t &d) noexcept
{
    insert(d);
}

RBtree::RBtree(const std::initializer_list<RBtree::data_t> &t)
{
    for (const auto &d : t)
    {
        insert(d);
    }
}

RBtree::RBtree(const RBtree &other)
{
    PreOrderCopy(other.root);
}

RBtree::RBtree(RBtree &&other) : root(other.root), _count(other._count)
{
    other._count = 0;
    other.root = nullptr;
}

RBtree &RBtree::operator=(RBtree &&other)
{
    if (this != &other)
    {
        std::swap(_count,other._count);
        std::swap(root,other.root);
    }
    return *this;
}

RBtree &RBtree::operator=(const RBtree &other)
{
    if (this != &other)
    {
        RBtree temp(other);
        std::swap(_count,temp._count);
        std::swap(root,temp.root);
    }
    return *this;
}

RBtree::key_t RBtree::get_key(const RBtree::data_t &elem) noexcept
{
    return std::get<0>(elem);
}

RBtree::value_t RBtree::get_value(const RBtree::data_t &elem) noexcept
{
    return std::get<1>(elem);
}

RBtree::data_t RBtree::get_root_data()
{
    return root->element;
}

RBtree::key_t RBtree::get_root_key()
{
    return get_key(root->element);
}

bool RBtree::empty() const noexcept
{
    return (root == nullptr);
}

size_t RBtree::count() const noexcept
{
    return _count;
}

int8_t RBtree::height() const noexcept
{
    return find_height(root);
}
// опускает узел ниже (используется для функций поворотов)
void RBtree::node_t::move_down(RBtree::node_t *y)
{
    if (parent != nullptr)
    {
        if (isLeftChild()) parent->left = y;
        else parent->right = y;
    }
    y->parent = parent;
    parent = y;
}
// левый поворот вокруг x
void RBtree::leftRotate(node_t *x) noexcept
{
    node_t* y = x->right;

    if (x == root) root = y;

    x->move_down(y);

    x->right = y->left;

    if (y->left != nullptr)  y->left->parent = x;

    y->left = x;
}

// правый поворот вокруг p
void RBtree::rightRotate(node_t *x) noexcept
{
    node_t* y = x->left;

    if (x == root) root = y;

    x->move_down(y);

    x->left = y->right;

    if (y->right != nullptr)  y->right->parent = x;

    y->right = x;
}

RBtree::data_t RBtree::find(const RBtree::key_t &key)
{
    if (empty())
    {
        throw std::length_error("Дерево пустое, поиск невозможен.");
    }
    data_t res = findNode(key, root)->element;
    // если нашли, то возвращаем результат, иначе возвращаем корень
    if (get_key(res) == key) return res;
    else return root->element;
}

RBtree::data_t RBtree::find_min()
{
    return find_min_node(root)->element;
}

RBtree::data_t RBtree::find_max()
{
    return find_max_node(root)->element;
}

bool RBtree::insert(const RBtree::key_t &key, const RBtree::value_t &v)
{
    if (check_for_unique_key(key))
    {
        add_node(key,v);
        _count++;
        return true;
    }
    return false;
}

bool RBtree::insert(const RBtree::data_t &d)
{
    auto [key,v] = d;
    return insert(key,v);
}

bool RBtree::replace(const RBtree::key_t &key, const RBtree::value_t &v)
{
    if (!empty())
    {
        auto toReplace = findNode(key, root);
        // элемент не найден
        if (get_key(toReplace->element) != key) return false;
        // если нашли
        toReplace->element = {key,v};
        return true;
    }
    return false;
}

bool RBtree::erase(const RBtree::key_t &key)
{
    size_t old_count = _count;
    auto toErase = findNode(key,root);
    removeNode(toErase);
    return (old_count != _count);
}

void RBtree::clear()
{
    erase_all(root);
    _count = 0;
}

void RBtree::swap(RBtree &t) noexcept
{
    std::swap(root, t.root);
    std::swap(_count, t._count);
}

void RBtree::PreOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoPreOrderRecurse(toWrite, root);
    toWrite.close();
}

void RBtree::InOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoInOrderRecurse(toWrite, root);
    toWrite.close();
}

void RBtree::PostOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoPostOrderRecurse(toWrite, root);
    toWrite.close();
}

void RBtree::PreOrderIterative(const std::string &filename) const noexcept
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
                    toWrite << elemToString(current->element) << " " << current->color << std::endl;
                    current = current->left;
                }
                current = stack.top()->right;
                stack.pop();
            }
        }
        toWrite.close();
    }
}

void RBtree::InOrderIterative(const std::string &filename) const noexcept
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
                toWrite << elemToString(stack.top()->element) << " " << stack.top()->color << std::endl;
                current = stack.top()->right;
                stack.pop();
            }
        }
        toWrite.close();
    }
}

void RBtree::PostOrderIterative(const std::string &filename) const noexcept
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
                toWrite << elemToString(stackR.top()->element) << " " << stackR.top()->color << std::endl; // запись данных в файл
                stackR.pop();
            }
        }
    }
}

void RBtree::concat(const RBtree &b)
{
    if (root != b.root)
    {
        PreOrderCopy(b.root);
    }
}

// прямой обход (сверху вниз)
void RBtree::DoPreOrderRecurse(std::ofstream &write, RBtree::node_t *node) const
{
    if (node != nullptr)
    {
        write << elemToString(node->element) << " " << node->color << std::endl;
        DoPreOrderRecurse(write, node->left);
        DoPreOrderRecurse(write, node->right);
    }
}

void RBtree::DoInOrderRecurse(std::ofstream &write, RBtree::node_t *node) const
{
    if (node != nullptr)
    {
        DoInOrderRecurse(write, node->left);
        write << elemToString(node->element) << " " << node->color << std::endl;
        DoInOrderRecurse(write, node->right);
    }
}

void RBtree::DoPostOrderRecurse(std::ofstream &write, RBtree::node_t *node) const
{
    if (node != nullptr)
    {
        DoPostOrderRecurse(write, node->left);
        DoPostOrderRecurse(write, node->right);
        write << elemToString(node->element) << " " << node->color << std::endl;
    }
}

RBtree::node_t *RBtree::findNode(const RBtree::key_t &key, RBtree::node_t *node)
{
    node_t* temp = node;
    // поиск продолжается пока не дойдем до элемента
    // или пока не доходим до листа, который должен был быть предполагаемым родителем
    // искомого элемента
    bool isSearchContinues = true;
    while (isSearchContinues)
    {
        if(key == get_key(temp->element))   // если нашли ключ
        {
            isSearchContinues = false;
        }
        else if(key < get_key(temp->element))
        {
            if (temp->left == nullptr) isSearchContinues = false;
            else temp = temp->left;
        }
        else
        {
            if (temp->right == nullptr) isSearchContinues = false;
            else temp = temp->right;
        }
    }
    return temp;
}

bool RBtree::check_for_unique_key(const RBtree::key_t &key)
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

int8_t RBtree::find_height(RBtree::node_t *node) const noexcept
{
    return node == nullptr ? 0 : (1 + std::max(find_height(node->left), find_height(node->right)));
}

void RBtree::PreOrderCopy(RBtree::node_t *node)
{
    if (node != nullptr)
    {
        insert(node->element);
        PreOrderCopy(node->left);
        PreOrderCopy(node->right);
    }
}

RBtree::node_t *RBtree::find_min_node(RBtree::node_t *node)
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

RBtree::node_t *RBtree::find_max_node(RBtree::node_t *node)
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

// эта функция как бы заменяет в связи u <-> u.parent, на v (v <-> u.parent)
void RBtree::transplant(RBtree::node_t *u, RBtree::node_t *v)
{
    if (u->parent == nullptr)
    {
        root = v;
    }
    else if (u->isLeftChild())
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != nullptr)
    {
        v->parent = u->parent;
    }
}

void RBtree::removeNode(node_t *z)
{
    if (z)
    {
        node_t* x = nullptr;
        node_t* y = z;
        nodeColor origColor = y->color;
        if (z->left && z->right) // два наследника
        {
            y = find_max_node(z->left);
            origColor = y->color;
            x = y->left;
            if (x == nullptr) // если дочернего узла не нашлось
            {
                x = y; // то поместим в x этот найденный максимальный узел
                       // для того чтобы исправить свойства дерева
                if (origColor == black) { fixAfterDelete(x); }
                x = nullptr;
            }
            if (y->parent != z)
            {
                transplant(y,y->left);
                y->left = z->left;
                y->left->parent = y;
            }
            transplant(z,y);
            y->right = z->right;
            y->right->parent = y;
            y->color = z->color;
        }
        else if (z->right != nullptr) // есть только правый наследник
        {
            x = z->right;
            transplant(z,z->right);
        }
        else if (z->left != nullptr) // только левый
        {
            x = z->left;
            transplant(z,z->left);
        }
        else // наследников нет
        {
            x = z;
            if (origColor == black) { fixAfterDelete(x); }
            transplant(z,nullptr);
            x = nullptr;
        }

        // начинаем фиксить свойства дерева, только если удаляли черный узел
        if (origColor == black && x != nullptr)
        {
            fixAfterDelete(x);
        }
        --_count;
        delete z;
    }
}

void RBtree::fixAfterInsert(RBtree::node_t *x)
{
    if (x == root)
    {
        x->color = black;
        return;
    }

    node_t* parent = x->parent;
    node_t* grandparent = parent->parent;
    node_t* uncle = x->uncle();

    if (parent->color == red)
    {
        // --- uncle КРАСНЫЙ --- //
        if (uncle != nullptr && uncle->color == red)
        {
            parent->color = black;
            uncle->color = black;
            grandparent->color = red;
            // идем восстанавливать свойства по узлам к корню дальше
            fixAfterInsert(grandparent);
        }
        else
        {
            // -- uncle ЧЕРНЫЙ -- //
            if (parent->isLeftChild()) // текущий узел в левом поддереве от деда
            {
                // 2 случай, если текущий узел в правом поддереве от отца
                if (!x->isLeftChild())
                {
                    // переходим от 2 случая к 3, поворотом вокруг parent
                    leftRotate(parent);
                    // учтем поворот в нашей локальной переменной parent
                    parent = x;
                }
                // 3 случай, если текущий узел в левом поддереве от отца
                // --- перекрашиваем и поворачиваем -- //
                parent->color = black;
                grandparent->color = red;
                rightRotate(grandparent);
            }
            else // тоже самое, но когда текущий узел в правом поддереве от деда (поэтому симметрично все делаем)
            {
                if (x->isLeftChild())
                {
                    rightRotate(parent);
                    parent = x;
                }
                parent->color = black;
                grandparent->color = red;
                leftRotate(grandparent);
            }
        }
    }
}
// фиксим свойства после удаления, случаев где-то 6 + 6 им симметричных
void RBtree::fixAfterDelete(RBtree::node_t *x)
{
    if (x == root) return;

    node_t* brother = x->brother();
    node_t* parent = x->parent;
    if (brother == nullptr)
    {
        fixAfterDelete(parent);
    }
    else
    {
        if (brother->color == red)
        {
            parent->color = red;
            brother->color = black;
            if (brother->isLeftChild()) rightRotate(parent);
            else leftRotate(parent);

            fixAfterDelete(x);
        }
        else
        {
            if (brother->has_red_child())
            {
                if (brother->left != nullptr && brother->left->color == red)
                {
                    if (brother->isLeftChild())
                    {
                        brother->left->color = brother->color;
                        brother->color = parent->color;
                        rightRotate(parent);
                    }
                    else
                    {
                        brother->left->color = parent->color;
                        rightRotate(brother);
                        leftRotate(parent);
                    }
                }
                else
                {
                    if (brother->isLeftChild())
                    {
                        brother->right->color = parent->color;
                        leftRotate(brother);
                        rightRotate(parent);
                    }
                    else
                    {
                        brother->right->color = brother->color;
                        brother->color = parent->color;
                        leftRotate(parent);
                    }
                }
                parent->color = black;
            }
            else
            {
                brother->color = red;
                if (parent->color == black) fixAfterDelete(parent);
                else parent->color = black;
            }
        }
    }
}

RBtree::node_t *RBtree::node_t::uncle() const
{
    // если нет родительского узла, то возвращаем nullptr
    if (!parent)  return nullptr;
    // если родительский узел есть, тогда возвращаем его брата
    return parent->brother();

}

RBtree::node_t *RBtree::node_t::brother() const
{
    // если отца нет, то и брата не будет
    if (!parent) return nullptr;
    // если текущий узел является левым дочерним узлом, то брат будет правым
    if (isLeftChild()) return parent->right;
    // и наоборот
    return parent->left;
}

bool RBtree::node_t::isLeftChild() const
{
    // если этот узел = левому дочернему для parent, значит да
    return (this == parent->left);
}

bool RBtree::node_t::has_red_child() const
{
    return (left != nullptr && left->color == red) || (right != nullptr && right->color == red);
}
