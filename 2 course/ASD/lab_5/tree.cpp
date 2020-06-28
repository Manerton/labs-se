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
    if(node != nullptr)				// ���� ���� ������ ����������
    {                               // ������� ������� ���� ����
        erase_all(node->left);	// ������� ��� ������ �������,
        erase_all(node->right);  // ����� ��� ������� �������

        // ����������� ������ � �������� ���������
        delete node;
        node = nullptr;
    }
}

void Tree::add_node(const Tree::key_t &key, const Tree::value_t &v, node_t *&node)
{
    if(node == nullptr)   // ���� ������ ��� ����
    {
        node = new node_t(data_t{key,v});
    }
    else
    {
        if(key < get_key(node->element))    // ���� ���� ������������ �������� ������ ������� ����
        {
            add_node(key, v, node->left);     // �� ���� �����
        }
        else
        {
            add_node(key, v, node->right);    // ����� ������
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
        throw std::length_error("������ ������, ����� ����������.");
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
        if (toWrite.is_open()) // ���� ������� ������ ������
        {
            std::stack<node_t*> stack; // ��� ������ ���������
            node_t* current = root;
            while (current != nullptr || !stack.empty()) // ���� �� ������ �� ���������� �������� ��� ���� �� ����
            {
                while (current != nullptr) // ���� ��������� �������
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
        if (toWrite.is_open()) // ���� ������� ������ ������
        {
            std::stack<node_t*> stack; // ��� ������ ���������
            node_t* current = root;
            while (current != nullptr || !stack.empty()) // ���� �� ������ �� ���������� �������� ��� ���� �� ����
            {
                while (current != nullptr) // ���� ��������� �������
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
            std::stack<node_t *> stack, stackR; // ��� ������ ���������
            node_t* current = root;
            stack.push(current);
            while (!stack.empty()) // ���� �� ������ �� ���������� �������� ��� ���� �� ����
            {
                current = stack.top();
                stack.pop();
                stackR.push(current);
                if (current->left)  stack.push(current->left);
                if (current->right) stack.push(current->right);
            }
            while(!stackR.empty()) // ����, ������� ���������� � ������� ��������
            {
                toWrite << elemToString(stackR.top()->element) << std::endl; // ������ ������ � ����
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

// ������ ����� (������ ����)
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
        if(key == get_key(node->element))   // ���� ����� ����
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
        if (l || r) // ���� ���� ���� ���� �������
        {
            // �������� ���� � ������ ��������� ����
            if((l) && (key == get_key(l->element)))
            {
                return node; // ���� ����� ����
            }
            // �������� ���� � ������� ��������� ����
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
        else return root;   // ����� �� �����, �� ���� �� ��� ����, ��� �� ������
    }
    else return root;   // ������ �� �����
}

bool Tree::check_for_unique_key(const Tree::key_t &key)
{
    if (!empty()) // ���� ������ �� ������, �� ���� ���� ���������
    {
        // ���� ��� ������ ������ (������� �� ����� �����), ��
        // ������ ���� ���������� � ��� ��� � ������
        // ����� ���� ������������
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
        throw std::length_error("������ ������, ����� ����������.");
    }

    // ����������� ������ � �������� ������ ������ � ������ ����� ����

    node_t* current = node;
    while (current->left)  current = current->left; // ��������� �����, ���� ����� ���� �������
    return current; // ������ ��������� �������
}

Tree::node_t *Tree::find_max_node(Tree::node_t *node)
{
    if (empty())
    {
        throw std::length_error("������ ������, ����� ����������.");
    }

    // ������������ ������ � �������� ������ ������ � ������ ������ ����

    node_t* current = node;
    while (current->right)  current = current->right; // ��������� �����, ���� ����� ���� �������
    return current; // ������ ��������� �������
}

void Tree::removeNode(Tree::node_t *parent, node_t *&target)
{
    if (target)
    {
        auto l = target->left;
        auto r = target->right;
        if (l && r) // ��� ���������� ����, ������ �������� ���� �� �������� ������ ���������
        {
            auto localMax = find_max_node(l);
            auto localMaxkey = get_key(localMax->element);
            target->element = localMax->element;
            // � ������� �������� ������ ���������, ������ ����� ����� ��� ���. ���� ����
            removeNode(findParentNode(localMaxkey,target),localMax);
            return;
        }
        else if (l) // ���� ������ ����� ���������
        {
            if (target == parent->left)
            {
                parent->left = target->left;
            } else
            {
                parent->right = target->left;
            }
        }
        else if (r) // ���� ������ ������ ���������
        {
            if (target == parent->right)
            {
                parent->right = target->right;
            } else
            {
                parent->left = target->right;
            }
        }
        else  // ��� �����������
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
