#include "btree.h"
#include <sstream>

BTree::node_t::node_t(int _t, bool _leaf) : t(_t), leaf(_leaf), n(0)
{
    // Копируем заданную минимальную степень и свойство листа
    // Распределяем память на максимальное количество возможных ключей
    // и дочерние указатели
    elements = new data_t[2*t-1];
    C = new node_t* [2*t];
    // Инициализируем количество ключей как 0
    n = 0;
}


// Ищем индекс первого ключа, который >= k
int BTree::node_t::findKey(key_t k)
{
    int i=0;
    while (i<n && key(i) < k) ++i;
    return i;
}

// Функция для удаления ключа k из поддерева this
bool BTree::node_t::remove(key_t k)
{
    int idx = findKey(k);
    // Ключ для удаления присутствует в этом узле
    if (idx < n && key(idx) == k)
    {
        // Если узел является листовым узлом - вызывается removeFromLeaf
        // В противном случае вызывается функция removeFromNonLeaf
        if (leaf) removeFromLeaf(idx);
        else removeFromNonLeaf(idx);
    }
    else
    {
        // Если этот узел является листовым и ключа до этого не нашли, то ключ отсутствует в дереве
        if (leaf) return false;
        // Ключ для удаления присутствует в поддереве с корнем этого узла
        // Флаг указывает, присутствует ли ключ в корне поддерева
        // с последним потомком этого узла
        bool flag = (idx == n);
        // если у дочернего элемента, где должен существовать ключ, меньше t ключей,
        // то заполняем этого ребенка (так как мы не можем удалить ключ, пока у него меньше чем t ключей)
        if (C[idx]->n < t) fill(idx);

        // Если дочерний элемент C[idx] был объединен с C[idx-1], теперь ключ в C[idx-1]
        // и поэтому мы возвращаемся к (idx-1)-ому дочернему элементу.
        // Иначе удаляем C[idx] который теперь имеет как минимум t ключей
        if (flag && idx > n)  C[idx-1]->remove(k);
        else C[idx]->remove(k);
    }
    return true;
}
// Функция для удаления ключа idx из узла, который является листом
void BTree::node_t::removeFromLeaf (int idx)
{
    // Сдвинуть все ключи после idx на одно место назад
    // тем самым удаляя ключ
    for (int i=idx+1; i<n; ++i)
    {
        //key(i-1) = key(i);
        elements[i-1] = elements[i];
    }
//    if (idx+1 == n) // удаляем последний элемент
//    {
//        elements[idx] = data_t{0,0};
//    }
    // Уменьшаем количество ключей
    --n;
}
// Функция для удаления ключа idx из узла, который не является листом (3 случая)
void BTree::node_t::removeFromNonLeaf(int idx)
{
    key_t k = key(idx);
    // 1 случай - если дочерний элемент (C[idx]), предшествующий k, имеет по крайней мере t ключей
    // то найти предшественника k ('pred') в поддереве с корнем C[idx]
    // после заменяем k на 'pred' и удаляем 'pred'
    if (C[idx]->n >= t)
    {
        data_t pred = getPred(idx);
        elements[idx] = pred;
        C[idx]->remove(get_key(pred));
    }
    // 2 - случай если у дочернего элемента C[idx] меньше t ключей, исследуем C[idx + 1]
    // если C [idx + 1] имеет по крайней мере t ключей, найдем 'sled' - следующий за k ключ в
    // поддереве, корнем которого является C[idx+1]
    // заменим k на succ и удалим sled
    //
    else if (C[idx+1]->n >= t)
    {
        data_t sled = getSled(idx);
        elements[idx] = sled;
        C[idx+1]->remove(get_key(sled));
    }
    // 3 случай - Если у обоих C [idx] и C [idx + 1] меньше t ключей,
    // тогда вносим k и все ключи из C[idx + 1] в C[idx]
    // и теперь C[idx] содержит 2t-1 ключей
    // освобождаем C[idx + 1] и удаляем k из C[idx]
    else
    {
        merge(idx);
        C[idx]->remove(k);
    }
}

// функция для получения ключа - предшественника для другого ключа
BTree::data_t BTree::node_t::getPred(int idx)
{
    // продолжаем двигаться к самому правому узлу, пока не достигнем листа
    node_t *cur = C[idx];
    while (!cur->leaf) cur = cur->C[cur->n];
    // Возвращаем последний ключ листа
    return cur->elements[cur->n-1];
}

// функция для получения ключа, который следует за другим ключом в узле
BTree::data_t BTree::node_t::getSled(int idx)
{
    node_t *cur = C[idx+1];
    while (!cur->leaf) cur = cur->C[0];
    return cur->elements[0];
}

// функция для заполнения дочернего C[idx], которая имеет менее чем t-1 ключей
void BTree::node_t::fill(int idx)
{
    // если предыдущий дочерний элемент (C[idx-1]) имеет более чем t-1 ключей
    // заимствуем ключ у него
    if (idx!=0 && C[idx-1]->n >= t) borrowFromPrev(idx);
    // а если следующий дочерний элемент (C [idx+1]) имеет более чем t-1 ключей, то
    else if (idx!=n && C[idx+1]->n >= t)  borrowFromNext(idx);

    // иначе слияние C[idx] с его родным братом
    // если C[idx] является последним потомком, объединяем его с предыдущим братом
    // в противном случае объединяем его со своим следующим братом
    else
    {
        if (idx != n)
            merge(idx);
        else
            merge(idx-1);
    }
}

// функция для заимствования ключа из C[idx-1]
void BTree::node_t::borrowFromPrev(int idx)
{
    node_t *child = C[idx];
    node_t *brother = C[idx-1];
    // в child надо вставить ключ, поэтому отодвигаем все ключи на шаг
    for (int i=child->n-1; i>=0; --i)
    {
        child->elements[i+1] = child->elements[i];
    }
    // Если child не является листом, отодвинем все его дочерние указатели
    if (!child->leaf)
    {
        for(int i=child->n; i >= 0; --i)
        {
            child->C[i+1] = child->C[i];
        }
    }
    // установка первого ключа для child из текущего узла (родительский узел)
    child->elements[0] = elements[idx-1];
    // перемещение последнего потомка brother как первого потомка child
    if(!child->leaf) child->C[0] = brother->C[brother->n];
    // перемещение ключа от brother к родителю
    elements[idx-1] = brother->elements[brother->n-1];
    ++child->n;
    --brother->n;
}

// функция для заимствования ключа из C[idx + 1]

void BTree::node_t::borrowFromNext(int idx)
{
    node_t *child = C[idx];

    node_t *brother = C[idx+1];

    // ключ вставляется как последний ключ в child из родителя
    child->elements[(child->n)] = elements[idx];
    // первый дочерний указатель на элемент брата вставляется как последний дочерний элемент в child
    if (!(child->leaf)) child->C[(child->n)+1] = brother->C[0];

    // Первый ключ от брата вставлен в keys [idx]
    elements[idx] = brother->elements[0];

    for (int i=1; i<brother->n; ++i) brother->elements[i-1] = brother->elements[i];

    if (!brother->leaf)
    {
        for(int i=1; i <= brother->n; ++i)

            brother->C[i-1] = brother->C[i];
    }

    // увеличиваем и уменьшаем количество ключей C [idx] и C [idx + 1]
    ++child->n;
    --brother->n;
}


// функция для объединения двух узлов, похоже на расщепление, но в обратную сторону
void BTree::node_t::merge(int idx)
{
    node_t *child = C[idx];
    node_t *brother = C[idx+1];
    // Вытащить ключ из текущего узла и вставить его в (t-1)-й
    child->elements[t-1] = elements[idx];
    // копирование ключей из brother в child в конец
    for (int i=0; i < brother->n; ++i) child->elements[i+t] = brother->elements[i];
    // копируем дочерние указатели
    if (!child->leaf)
    {
        for(int i=0; i <= brother->n; ++i)
        {
            child->C[i+t] = brother->C[i];
        }
    }
    // сдвигаем ключи влево чтобы заполнить пробел, созданный перемещением ключа из этого узла в child
    for (int i=idx+1; i < n; ++i) elements[i-1] = elements[i];
    // сдвигаем дочерние указатели после (idx + 1) в текущем узле вправо
    for (int i=idx+2; i <= n; ++i) C[i-1] = C[i];
    // Обновление счетчика ключей дочернего и текущего узла
    child->n += brother->n+1;
    --n;
    // Освобождаем память, занятую родным братом
    delete(brother);
}



bool BTree::check_for_unique_key(const key_t &key)
{
    if (root !=nullptr) // если корень не пустой, то ключ надо проверить
    {
        auto res = search(key);
        return (res == nullptr); // не был найден ключ
    }
    return true;
}
// Вставляем новый ключ в B-дерево
bool BTree::insert(const data_t &elem)
{
    key_t k = get_key(elem);
    if (check_for_unique_key(k)) // проверяем ключ на уникальность
    {
        // Если дерево пусто
        if (root == nullptr)
        {
            // Выделим память для root
            root = new node_t(t, true);
            root->elements[0] = elem;
            root->n = 1;  // Обновляем количество ключей в корне
        }
        else // Если дерево не пустое
        {
            // Если root полон, то дерево растет в высоту
            if (root->n == 2*t-1)
            {
                // Выделим память для нового корня
                node_t *s = new node_t(t, false);
                // Сделать старый корень дочерним для нового корня
                s->C[0] = root;
                // Разделить старый корень и переместить медиану (средний ключ) в новый корень
                s->splitChild(0, root);
                // Новый корень имеет двоих детей.
                // Решаем, у какого из двух детей будет новый ключ
                int i = 0;
                if (s->key(0) < k) i++;

                s->C[i]->insertNonFull(elem);
                // Изменить корень
                root = s;
            }
            else
            {
                // Если root не заполнен, вызывать insertNonFull для root
                root->insertNonFull(elem);
            }
        }
        return true;
    }
    return false;
}

bool BTree::replace(const BTree::data_t &elem)
{
    if (root != nullptr)
    {
        auto toReplace = search(get_key(elem));
        // элемент найден
        if (toReplace != nullptr)
        {
            int i=0;
            while (i < toReplace->n && toReplace->key(i) != get_key(elem)) ++i;
            toReplace->elements[i] = elem;
            return true;
        }
    }
    return false;
}


// вставка ключа k в дерево с незаполненным корневым узлом
void BTree::node_t::insertNonFull(const data_t &elem)
{
    key_t k = get_key(elem);
    // индекс как индекс самого правого элемента
    int i = n-1;
    // если это лист
    if (leaf == true)
    {
        // следующий цикл выполняет две вещи
        // а) находит местоположение нового ключа для вставки
        // б) перемещает все большие ключи на одно место вперед
        while (i >= 0 && key(i) > k)
        {
            elements[i+1] = elements[i];
            --i;
        }
        // вставляем новый ключ в найденное место
        elements[i+1] = elem;
        ++n;
    }
    else // Если этот узел не лист
    {
        // Найти ребенка, в который можно вставить новый ключ
        while (i >= 0 && key(i) > k) --i;

        ++i;

        // Посмотрим, заполнен ли найденный ребенок
        if (C[i]->n == 2*t-1)
        {
            // Если ребенок полон, то разделить его
            splitChild(i, C[i]);
            // После разделения медиана поднимается и C[i] разбивается на две части.
            // Выясняем, в какую часть (левую или правую) нам нужно вставить ключ
            if (key(i) < k) i++;
        }
        C[i]->insertNonFull(elem);
    }
}

// вспомогательная функция для разделения потомка "y" этого (this) узла
void BTree::node_t::splitChild(int i, node_t *y)
{
    // изначально в узле было 2*t-1 ключей
    // создаем новый узел, который будет хранить (t-1) бОльших ключей из "у"
    node_t *z = new node_t(y->t, y->leaf);
    z->n = t - 1;

    // Копируем последние (t-1) ключи y в z
    for (int j = 0; j < t-1; ++j)
    {
        z->elements[j] = y->elements[j+t];
    }
    // Если "y" не лист, то копируем его t потомков в z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; ++j)
        {
            z->C[j] = y->C[j+t];
        }
    }
    // Уменьшаем количество ключей в y
    y->n = t - 1;
    // Поскольку у этого узла будет новый дочерний элемент, предоставляем ему пространство
    for (int j = n; j >= i+1; --j)
    {
        C[j+1] = C[j];
    }
    // Вставляем z в качестве дочернего узла x (x - родитель y)
    C[i+1] = z;
    // на место i в этом узле должен встать серединный элемент (медиана)
    // тоже предоставляем этому ключу-медиане пространство
    for (int j = n-1; j >= i; j--)
    {
         elements[j+1] = elements[j];
    }
    // Копируем медиану в этот узел
    elements[i] = y->elements[t-1];
    // Увеличиваем количество ключей в этом узле
    n = n + 1;
}


// Функция для прохождения всех узлов в поддереве с корнем из этого узла

void BTree::node_t::traverse()

{
    // есть n ключей и n + 1 потомков, обходы через n ключей
    // и первые n детей
    int i;
    for (i = 0; i < n; i++)
    {
        // Если это не лист, то перед печатью ключ [i],
        // пройти поддерево с корнем дочернего C [i].
        if (leaf == false) C[i]->traverse();
        cout << " " << key(i);
    }
    // Распечатать поддерево с корнем из последнего потомка
    if (leaf == false) C[i]->traverse();

}
// для обхода в ширину (по уровням высоты)
std::string BTree::node_t::traverseIterative()
{
    stringstream ss;
    std::queue<node_t*> queue;
    queue.push(this);
    while (!queue.empty())
    {
        node_t* current = queue.front();
        queue.pop();
        int i;
        for (i = 0; i < current->n; i++)
        {
            if (current->leaf == false)
                queue.push(current->C[i]);
            ss << " " << current->key(i);
        }
        ss << " /"; // вставляем разделитель, чтобы различать ноды между собой при выводе
        if (current->leaf == false)
            queue.push(current->C[i]);
    }
    return ss.str();
}


// Функция для поиска ключа k в поддереве с этим узлом
BTree::node_t *BTree::node_t::search(key_t k)
{
    // Находим первый ключ больше или равный k
    int i = 0;
    while (i < n && k > key(i)) i++;

    // Если найденный ключ равен k, вернуть этот узел
    if (i < n && key(i) == k) return this;

    // Если ключ не найден здесь и это листовой узел
    if (leaf == true) return nullptr;

    // Перейти к соответствующему ребенку
    return C[i]->search(k);
}

bool BTree::remove(key_t k)
{
    if (!root)
    {
        return false;
    }

    // Вызываем функцию удаления для root
    bool res = root->remove(k);

    // Если в корне 0 ключей, то сделаем его первым дочерним узлом нового корня
    // если у него есть дочерний элемент, а в противном случае установим корень как nullptr
    if (root->n == 0)
    {
        node_t *tmp = root;
        if (root->leaf) root = nullptr;
        else root = root->C[0];
        // Освободим старый корень
        delete tmp;
    }
    return res;
}
