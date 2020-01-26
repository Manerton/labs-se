#include <iostream>
#include <vector>

using namespace std;

void sort_binary_vstavkami (vector<int> &a)
{
   size_t N = a.size();
   for (size_t i = 1; i < N; ++i)
   {
       int x = a[i];
       size_t L = 0;
       size_t R = i;
       while (L < R)
       {
           size_t m = (L+R) / 2;
           if (a[m] <= x) L = m+1;
           else R = m;
       }
       for (size_t j = i; j >= R+1; --j)
       {
           a[j] = a[j-1];
       }
       a[R] = x;
   }
}

void fixHeap( std::vector<int> &heap, int i, int size )
{
// Индекс максимального элемента в текущей тройке элементов:
    int maxIdx = i ;
// Значение текущего элемента:
    int value = heap[i];
    while ( true )
    {
        int childIdx = i * 2 + 1; //Индекс левого потомка
// Если есть левый потомок и он больше текущего элемента,
        if ( ( childIdx < size ) && ( heap[ childIdx ] > value ) )
            maxIdx = childIdx; // то он считается максимальным

        childIdx = i * 2 + 2; //Индекс правого потомка

// Если есть правый потомок и он больше максимального,

        if ( ( childIdx < size ) && ( heap[ childIdx ] > heap[ maxIdx ] ) )

            maxIdx = childIdx; // то он считается максимальным

// Если текущий элемент является максимальным из трёх

// (т.е. если он больше своих детей), то конец:
        if ( maxIdx == i )
            break;
// Меняем местами текущий элемент с максимальным:
        heap[i] = heap[ maxIdx ];
        heap[ maxIdx ] = value;
// Переходим к изменившемуся потомку
        i = maxIdx;
    }
}

// Пирамидальная сортировка массива heap размера size

void heapSort( std::vector<int> &heap )
{
// Построение пирамиды из массива:
    int size = heap.size();
    for( int i = size / 2 - 1; i >= 0; --i )
        fixHeap( heap, i, size);
// Сортировка с помощью пирамиды
    while( size > 1 ) // пока в пирамиде больше одного элемента
    {
        --size; // Отделяем последний элемент
// Обмениваем местами корневой элемент и отделённый:
        double firstElem = heap[0];
        heap[0] = heap[ size ];
        heap[ size ] = firstElem;
// "Просеиваем" новый корневой элемент вниз:
        fixHeap( heap, 0, size );
    }
}

void quick_sort(std::vector<int> &a, int first, int last)
{
    if (first < last)
    {
        int left = first, right = last, middle = a[(first+last)/2];
        do
        {
            while (a[left] < middle) ++left;
            while (a[right] > middle) --right;
            if (left <= right)
            {
                swap(a[left], a[right]);
                ++left;
                --right;
            }
        } while (left <= right);
        quick_sort(a,first,right);
        quick_sort(a,left,last);
    }
}

size_t lineyniy_poisk(const vector<int> &a, int x)
{
    size_t N = a.size();
    size_t i = 0;
    while (i < N && a[i] != x)
    {
        ++i;
    }
    return i;
}

size_t binary_poisk(const vector<int> &a, int x)
{
    size_t N = a.size();
    size_t L = 0;
    size_t R = N-1;
    size_t m = (L+R) / 2;
    while (L <= R && a[m] != x)
    {
        if (a[m] < x)
        {
            L = m+1;
        }
        else
        {
            R = m-1;
        }
        m = (L+R) / 2;
    }
    return m;
}

void sort_viborom (vector<int> &a)
{
    size_t N = a.size();
    for (size_t i = 0; i < N-1; ++i)
    {
        size_t k = i;
        int x = a[i];
        for (size_t j = i+1; j < N; ++j)
        {
            if (a[j] < x)
            {
                k = j;
                x = a[j];
            }
        }
        a[k] = a[i];
        a[i] = x;
    }
}

void sort_bubble_ltr (vector<int> &a)   // -- left to right -- //
{
    size_t N = a.size();
    for (size_t i = 1; i < N; ++i)
    {
        for (size_t j = 0; j < N-i; ++j)
        {
            if (a[j] > a[j+1])
            {
                swap(a[j],a[j+1]);
            }
        }
    }
}

void sort_bubble_rtl (vector<int> &a)   // -- right to left -- //
{
    size_t N = a.size();
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = N-1; j > i; --j)
        {
            if (a[j-1] > a[j])
            {
                swap(a[j-1],a[j]);
            }
        }
    }
}

void sort_shella (vector<int> &a)
{
    size_t N = a.size();
    size_t d = N / 2;
    while (d > 0)
    {
        for (size_t i = d; i < N; ++i)
        {
            int x = a[i];
            size_t j = i-d;
            while (j >= d && x < a[j])
            {
                a[j+d] = a[j];
                j -= d;
            }
            if (j >= d || x >= a[j])
            {
                a[j+d] = x;
            }
            else
            {
                a[j+d] = a[j];
                a[j] = x;
            }
        }
        d /= 2;
    }
}

void sort_vstavkami (vector<int> &a)
{
    size_t N = a.size();
    for (size_t i = 1; i < N; ++i)
    {
        int x = a[i];
        size_t j = i;
        while (j > 0 && x < a[j-1])
        {
            a[j] = a[j-1];
            --j;
        }
        a[j] = x;
    }
}


void display (const vector<int> &v)
{
    for (int val : v)
    {
        cout << val << " ";
    }
    cout << endl;
}

int main()
{
    vector<int> a = {32,64,9,30,87,14,2,76};
    display(a);
    int *A = new int [15];
    A[3] = 7;
    A[5] = 12;
    A[14] = -3;
    int *B = new int [1635];
    B[3] = 7;
    B[5] = 12;
    B[14] = -3;
    B[15] = -44;
    delete []A;
    A = B;
    cout << A[3] << " " << A[5] << " " << A[14] << " " << A[15];
    heapSort(a);
    //display(a);

    return 0;
}
