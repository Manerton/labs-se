#include <iostream>

using namespace std;

#include <iostream>
#include <random>
#include <ctime>


using namespace std;

/**< на массиве */
class deque
{
private:
    int *A = nullptr;
    size_t _size = 0;
    public:
    class size_exception{};
    bool empty(){
        return (_size == 0);}

    void clear(){
        _size = 0;
        delete[] A;
        }

    size_t size()
    {
        return _size;
    }

    int front()
    {
        if(_size != 0) return A[0];
        throw size_exception();
    }
    int back()
    {
        if(_size != 0) return A[_size-1];
        throw size_exception();
    }

    void push_back(const int &N)
    {
        int *B = new int[_size+1];
        std::move(A, A+_size,B);
        B[_size] = N;
        _size++;
        delete[] A;
        A = B;
    }
    void push_front(const int &N)
    {
        int *B = new int[_size+1];
        std::move(A, A+_size,B+1);
        B[0] = N;
        _size++;
        delete[] A;
        A = B;
    }
    void pop_front()
    {
        if(_size != 0)
        {
            int *B = new int[_size-1];
            std::move(A+1,A+_size,B);
            _size--;
            delete[] A;
            A = B;
        }
    }
    void pop_back()
    {
        if(_size != 0)
        {
            _size--;
            int *B = new int[_size];
            std::move(A,A+_size,B);
            delete[] A;
            A = B;
        }
    }
    friend std::ostream& operator<<(std::ostream& t, const deque &r)
    {
        if(r._size == 0) return t;
        for(size_t i = 0; i < r._size; i++) t << r.A[i]<<" ";
        std::cout << std::endl;
        return t;
    }

    friend std::istream& operator>>(std::istream& t, deque &r)
    {
        int j = 0;
        for(size_t i = 0; i < r._size; i++) { t >> j; r.push_back(j); }
        std::cout << std::endl;
        return t;
    }
};

/**< на списке*/


class dequeS
{
private:
    struct node
    {
        int N;
        node *next = nullptr;
        node *prev = nullptr;
    };
    node *head = nullptr;
    node *tail = nullptr;
    size_t _size = 0;
public:
    class size_exception{};

    bool empty(){
        return (_size == 0);}

    void clear()
    {
        while (!empty()) pop_front();
    }

    size_t size()
    {
        return _size;
    }

    int front()
    {
        if(_size != 0) return head->N;
        throw size_exception();
    }
    int back()
    {
        if(_size != 0) return tail->N;
        throw size_exception();
    }

    void push_back(const int &N)
    {
        node* newNode = new node;
        newNode->N = N;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        if (empty()) // если очередь пустая
        {
            head = newNode;
            tail = head;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        _size++;
    }
    void push_front(const int &N)
    {
        node* newNode = new node;
        newNode->N = N;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        if (empty()) // если очередь пустая
        {
            head = newNode;
            tail = head;
        }
        else
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        _size++;
    }
    void pop_front()
    {
        if(!empty())
        {
            node* toDelete = head;
            if (head == tail)   // в очереди только один узел
            {
                head = nullptr;
                tail = nullptr;
            }
            else
            {
                head = head->next;
                head->prev = nullptr;
            }
            delete toDelete;
            _size--;
        }
    }
    void pop_back()
    {
        if(!empty())
        {
            node* toDelete = tail;
            if (head == tail)   // в очереди только один узел
            {
                head = nullptr;
                tail = nullptr;
            }
            else
            {
                tail = tail->prev;
                tail->next = nullptr;
            }
            delete toDelete;
            _size--;
        }

    }
    friend std::ostream& operator<<(std::ostream& t, const dequeS &r)
    {
        for(node* i = r.head; i != nullptr; i = i->next) t << i->N<<" ";
        std::cout << std::endl;
        return t;
    }

};



int main()
{
    cout << "Hello World!" << endl;
    dequeS A;
    A.push_back(5);
    A.push_back(7);
    A.push_back(8);
    A.push_back(-3);
    A.push_front(0);
    cout << A;
    A.pop_back();
    A.pop_back();
    cout << A;
    A.pop_front();
    cout << A;
    A.pop_front();
     cout << A;
     A.pop_back();
     cout << A;
    cout << "da";
    return 0;
}
