#include "tree.h"
#include "doctest.h"
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stack>

std::string elemToString(const AVLtree::data_t &element)
{
    std::stringstream ss;
    ss << AVLtree::get_key(element) << " "
       << int(std::get<0>(AVLtree::get_value(element))) << " "
       << std::get<1>(AVLtree::get_value(element)) << " "
       << int(std::get<2>(AVLtree::get_value(element))) << " "
       << std::get<3>(AVLtree::get_value(element)) << " "
       << int(std::get<4>(AVLtree::get_value(element))) << " "
       << std::get<5>(AVLtree::get_value(element));
    return ss.str();
}

void AVLtree::erase_all(AVLtree::node_t *&node)
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

void AVLtree::add_node(const AVLtree::key_t &key, const AVLtree::value_t &v, node_t* &node)
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
        node = balance(node);
    }
}

AVLtree::~AVLtree()
{
    clear();
}

AVLtree::AVLtree(const AVLtree::data_t &d) noexcept
{
    insert(d);
}

AVLtree::AVLtree(const std::initializer_list<AVLtree::data_t> &t)
{
    for (const auto &d : t)
    {
        insert(d);
    }
}

AVLtree::AVLtree(const AVLtree &other)
{
    PreOrderCopy(other.root);
}

AVLtree::AVLtree(AVLtree &&other) : root(other.root), _count(other._count)
{
    other._count = 0;
    other.root = nullptr;
}

AVLtree &AVLtree::operator=(AVLtree &&other)
{
    if (this != &other)
    {
        std::swap(_count,other._count);
        std::swap(root,other.root);
    }
    return *this;
}

AVLtree &AVLtree::operator=(const AVLtree &other)
{
    if (this != &other)
    {
        AVLtree temp(other);
        std::swap(_count,temp._count);
        std::swap(root,temp.root);
    }
    return *this;
}

AVLtree::key_t AVLtree::get_key(const AVLtree::data_t &elem) noexcept
{
    return std::get<0>(elem);
}

AVLtree::value_t AVLtree::get_value(const AVLtree::data_t &elem) noexcept
{
    return std::get<1>(elem);
}

AVLtree::data_t AVLtree::get_root_data()
{
    return root->element;
}

AVLtree::key_t AVLtree::get_root_key()
{
    return get_key(root->element);
}

bool AVLtree::empty() const noexcept
{
    return (root == nullptr);
}

size_t AVLtree::count() const noexcept
{
    return _count;
}

int8_t AVLtree::height(node_t* node) const noexcept
{
    return node?node->_height:0;
}

int8_t AVLtree::height() const noexcept
{
    return height(root);
}
// правый поворот вокруг p
AVLtree::node_t *AVLtree::rightRotate(node_t *p) noexcept
{
    node_t* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}
// левый поворот вокруг q
AVLtree::node_t *AVLtree::leftRotate(AVLtree::node_t *q) noexcept
{
    // левый поворот является симметричной копией правого:
    node_t* p = q->right;
    q->right = p->left;
    p->left = q;
    fixHeight(q);
    fixHeight(p);
    return p;
}
// балансировка узла p
AVLtree::node_t *AVLtree::balance(node_t *p) noexcept
{
    if (p != nullptr)
    {
        fixHeight(p);
        if( bFactor(p)==2 )
        {
            // если высота левого поддерева узла p->right больше высоты его правого поддерева
            if( bFactor(p->right) < 0 )
            {
                p->right = rightRotate(p->right);
            }
            return leftRotate(p);
        }
        if( bFactor(p)==-2 )
        {
            if( bFactor(p->left) > 0  )
            {
                p->left = leftRotate(p->left);
            }
            return rightRotate(p);
        }
    }
    return p; // балансировка не нужна
}

AVLtree::data_t AVLtree::find(const AVLtree::key_t &key)
{
    if (empty())
    {
        throw std::length_error("Дерево пустое, поиск невозможен.");
    }
    return findNode(key, root)->element;
}

AVLtree::data_t AVLtree::find_min()
{
    return find_min_node(root)->element;
}

AVLtree::data_t AVLtree::find_max()
{
    return find_max_node(root)->element;
}

bool AVLtree::insert(const AVLtree::key_t &key, const AVLtree::value_t &v)
{
    if (check_for_unique_key(key))
    {
        add_node(key,v,root);
        _count++;
        return true;
    }
    return false;
}

bool AVLtree::insert(const AVLtree::data_t &d)
{
    auto [key,v] = d;
    return insert(key,v);
}

bool AVLtree::replace(const AVLtree::key_t &key, const AVLtree::value_t &v)
{
    if (!empty())
    {
        auto toReplace = findNode(key, root);
        toReplace->element = {key,v};
        return true;
    }
    return false;
    }

bool AVLtree::erase(const AVLtree::key_t &key)
{
    size_t old_count = _count;
    removeNode(key,root);
    return (old_count != _count);
}

void AVLtree::clear()
{
    erase_all(root);
    _count = 0;
}

void AVLtree::swap(AVLtree &t) noexcept
{
    std::swap(root, t.root);
    std::swap(_count, t._count);
}

void AVLtree::PreOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoPreOrderRecurse(toWrite, root);
    toWrite.close();
}

void AVLtree::InOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoInOrderRecurse(toWrite, root);
    toWrite.close();
}

void AVLtree::PostOrderRecurse(const std::string &filename) const noexcept
{
    std::ofstream toWrite(filename);
    if (toWrite.is_open()) DoPostOrderRecurse(toWrite, root);
    toWrite.close();
}

void AVLtree::PreOrderIterative(const std::string &filename) const noexcept
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

void AVLtree::InOrderIterative(const std::string &filename) const noexcept
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

void AVLtree::PostOrderIterative(const std::string &filename) const noexcept
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

void AVLtree::concat(const AVLtree &b)
{
    if (root != b.root)
    {
        PreOrderCopy(b.root);
    }
}

// прямой обход (сверху вниз)
void AVLtree::DoPreOrderRecurse(std::ofstream &write, AVLtree::node_t *node) const
{
    if (node != nullptr)
    {
        write << elemToString(node->element) << std::endl;
        DoPreOrderRecurse(write, node->left);
        DoPreOrderRecurse(write, node->right);
    }
}

void AVLtree::DoInOrderRecurse(std::ofstream &write, AVLtree::node_t *node) const
{
    if (node != nullptr)
    {
        DoInOrderRecurse(write, node->left);
        write << elemToString(node->element) << std::endl;
        DoInOrderRecurse(write, node->right);
    }
}

void AVLtree::DoPostOrderRecurse(std::ofstream &write, AVLtree::node_t *node) const
{
    if (node != nullptr)
    {
        DoPostOrderRecurse(write, node->left);
        DoPostOrderRecurse(write, node->right);
        write << elemToString(node->element) << std::endl;
    }
}

AVLtree::node_t *AVLtree::findNode(const AVLtree::key_t &key, AVLtree::node_t *node)
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

bool AVLtree::check_for_unique_key(const AVLtree::key_t &key)
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

int8_t AVLtree::find_height(AVLtree::node_t *node) const noexcept
{
    return node == nullptr ? 0 : (1 + std::max(find_height(node->left), find_height(node->right)));
}

void AVLtree::PreOrderCopy(AVLtree::node_t *node)
{
    if (node != nullptr)
    {
        insert(node->element);
        PreOrderCopy(node->left);
        PreOrderCopy(node->right);
    }
}

AVLtree::node_t *AVLtree::find_min_node(AVLtree::node_t *node)
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

AVLtree::node_t *AVLtree::find_max_node(AVLtree::node_t *node)
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
// удаляем минимальный узел из дерева для функции removeNode, память для узла не освобождается
// потому-что этот удаленный минимальный узел встанет на место удаляемого узла в removeNode
AVLtree::node_t *AVLtree::remove_min_node(AVLtree::node_t *p)
{
    if( p->left == nullptr )
    {
        return p->right;
    }
    p->left = remove_min_node(p->left);
    return balance(p);
}

// обрабатываются все ситуации
// 1) У узла нет наследников (удаляем лист).
// 2) У узла один наследник.
// 3) У узла оба наследника.
AVLtree::node_t *AVLtree::removeNode(const key_t &key, node_t *p)
{
    if( p == nullptr ) return nullptr;
    // ищем нужный узел
    if( key < get_key(p->element) )
    {
        p->left = removeNode(key,p->left);
    }
    else if( key > get_key(p->element) )
    {
        p->right = removeNode(key,p->right);
    }
    else // найден ключ
    {
        node_t* q = p->left;
        node_t* r = p->right;
        delete p;   // удаляем узел
        _count--;
        // если правое поддерево пустое, то возвращаем указатель на левое поддерево
        // и если у p был наследник слева, то вернется он
        // ну а если p был листом, вернется nullptr
        if( r == nullptr ) return q;
        // обрабатываем случай, когда два наследника у p
        // или один, но справа
        node_t* min = find_min_node(r);
        min->right = remove_min_node(r);
        min->left = q;
        return balance(min);    // возвращаем новый сбалансированный узел
    }
    return balance(p);
}


int8_t AVLtree::bFactor(const AVLtree::node_t *node) const
{
    if (node == nullptr) throw std::logic_error("Баланс фактор невозможно вычислить, поскольку узла не существует!");
    return height(node->right) - height(node->left); // если 2 или -2, то возникла расбалансировка поддерева
}

void AVLtree::fixHeight(AVLtree::node_t *node)
{
    int8_t hl = height(node->left);
    int8_t hr = height(node->right);
    node->_height = (hl>hr?hl:hr)+1;
}
