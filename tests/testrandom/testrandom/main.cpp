#include <iostream>
#include <vector>

using namespace std;

class arr1
{
    double chislo = 0;
public:
//    void *operator new(size_t size)
//    {
//        printf("videlino %d bytes\n", size);
//        return malloc(size);
//    }
//    void operator delete(void *p)
//    {
//        printf("free memory for 1 elem\n");
//        free(p);
//    }
    void *operator new[](size_t size)
    {
        printf("videlino %d bytes\n", size);
        return malloc(size);
    }

    void operator delete[](void *p)
    {
        printf("free memory\n");
        free(p);
    }

    arr1(){cout << "constructor" << endl;}
    ~arr1(){cout << "destructor" << endl;} // а без него 24 байта
};

class arr2
{
    double chislo[3];
public:
    void *operator new(size_t size)
    {
        printf("videlino %d bytes\n", size);
        return malloc(size);
    }
    void operator delete(void *p)
    {
        printf("free memory for 1 elem\n");
        free(p);
    }
    void *operator new[](size_t size)
    {
        printf("videlino %d bytes\n", size);
        return malloc(size);
    }

    void operator delete[](void *p)
    {
        printf("free memory\n");
        free(p);
    }

    arr2(){cout << "constructor" << endl;}
    ~arr2(){cout << "destructor" << endl;}
};

class test {
public:
    arr1 A[3];
    //arr1 *ptr = new arr1 [3];
};

int main()
{
    arr1 *ptr = new arr1[3];
    delete [] ptr;


//    arr2 obj;
//    printf("%d\n", sizeof(obj));


//    test test1;
//    printf("%d\n", sizeof(test1));
    return 0;
}


