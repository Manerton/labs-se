// ver 1.1 - 06.04.19
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

void filling_array_from_keyboard (short int A[], int N)
{
    cout << "***        ***" << endl;
    cout << "    : " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << " " << i+1 << "-: ";
        cin >> chislo;
        A[i] = chislo;
    }
}

void filling_array_random (short int A[], int N, int min, int max)
{
    cout << "***        ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        A[i] = chislo;
    }
}

void filling_array_from_file (short int A[], int N, string filename)
{
    cout << "***       " << filename << " ***" << endl;
    int chislo;
    ifstream input(filename);
    if (input) //  
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            A[i] = chislo;
        }
        input.close();
    } else cout << endl << "error!     " << filename << endl;
}

void print_array (const short int A[], int N)
{
    for (int i = 0; i < N; i++) cout << A[i] << " ";
    cout << endl;
}

void diapazon_rand (int &min, int& max)
{
        cout << "    ." << endl;
        cout << " : ";
        cin >> min;
        cout << " : ";
        cin >> max;
        while (min >= max)
        {
            cout << "error!" << endl;
            cout << " : ";
            cin >> max;
        }
}

void sposob_zapolneniya (short int A[], int N)
{
    int choice;
    cout << "***    ***" << endl;
    cout << "1.   ." << endl;
    cout << "2.   ." << endl;
    cout << "3.   ." << endl;
    cout << "  : ";
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "error!" << endl;
        cout << "  : ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        filling_array_from_keyboard(A, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_array_random(A,N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "  : ";
        cin >> filename;
        filling_array_from_file(A,N,filename);
        break;
    }
}

//    (  double      int,   -  )
bool full_square (short int chislo)
{
    double sqrt_chislo = sqrt(chislo);
    if (chislo >= 0 && sqrt_chislo == static_cast<short int> (sqrt_chislo)) return true;
    else return false;
}

//     
short int first_full_square (const short int A[], int N)
{
    for(int i = 0; i < N; i++)
    {
        if (full_square( A[i] )) return A[i];
    }
    return -1; //      
}

//     
bool is_fibonachi (short int chislo)
{
    int summa(0), a1 (0), a2(1);
    while (summa < chislo)
    {
        summa = a1 + a2;
        a1 = a2;
        a2 = summa;
    }
    if (chislo == summa) return true;
    else return false;

}
// var - ,     
bool find_var (short int chislo, short int zadannaya_cifra)
{
    short int ostatok;
    if (chislo == zadannaya_cifra) return false;
    while (chislo > 0)
    {
        ostatok = chislo % 10;
        if (ostatok == zadannaya_cifra) return false;
        chislo /= 10;
    }
    return true;
}
//  ( temp  A)
void Copy(const short int temp[], short int A[], int N)
{
    for (int i = 0; i < N; ++i)
        A[i] = temp[i];
}
//     
int insert_future_size_of_array (const short int A[], int N, short int zadannaya_cifra)
{
    int N_out = N;
    for (int i = 0; i < N; i++)
    {
        if (find_var(A[i],zadannaya_cifra)) N_out++;
    }
    return N_out;
}
//      ,     
void insert_after_var (const short int A[], short int temp[], int N, short int fullsquare, short int zadannaya_cifra)
{
    int i_temp = 0;
    for (int i = 0; i < N; i++)
    {
        temp[i_temp] = A[i];
        if (find_var(A[i],zadannaya_cifra))
        {
            i_temp++;
            temp[i_temp] = fullsquare;
        }
        i_temp++;
    }
}
//   ,     
void insert_before_var (const short int A[], short int temp[], int N, short int fullsquare, short int zadannaya_cifra)
{
    int i_temp = 0;
    for (int i = 0; i < N; i++)
    {
        if (find_var(A[i],zadannaya_cifra))
        {
            temp[i_temp] = fullsquare;
            i_temp++;
            temp[i_temp] = A[i];
        }
        else temp[i_temp] = A[i];
        i_temp++;
    }
}
//     
int delete_future_size_of_array (const short int A[], int N)
{
    int N_out = N;
    for (int i = 0; i < N; i++)
    {
        if (is_fibonachi(A[i]))
        {
            N_out--;
        }
    }
    return N_out;
}
//   
void delete_fibonachi (const short int A[], short int temp[], int N)
{
    int i_temp = 0;
    for (int i = 0; i < N; i++)
    {
        if (!is_fibonachi(A[i]))
        {
            temp[i_temp] = A[i];
            i_temp++;
        }
    }
}

void display_menu ()
{
    cout << "***  ***" << endl;
    cout << "1.  1.1 -   ,     ,     ." << endl;
    cout << "2.  1.2 -   ,     ,     ." << endl;
    cout << "3.  2 -    ." << endl;
    cout << "4.   ." << endl;
}
int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "  6  13.\n:  . -11.\n" << endl;

    //    
    display_menu();

    //   
    int N;
    cout << endl << "   : ";
    cin >> N;
    //   
    while (N < 1)
    {
        cout << "error!" << endl;
        cout << "   : ";
        cin >> N;
    }
    //  

    short int *A = new short int[N]; //           N
    short int *temp; //     

    //  (, )   
    sposob_zapolneniya(A,N);
    cout << "  : ";
    print_array(A,N);

    int menu;

    while (menu != 4 && N > 0)
    { //       > 0 -  
        display_menu(); // 
        cout << "  : ";
        cin >> menu; //      
        //  
        while (menu < 0 || menu > 4)
        {
            cout << "error!" << endl;
            cout << "  : ";
            cin >> menu;
        }
        if (menu != 4) //      4  
        {
            switch (menu) //     
            {
                case 1:
                {
                    cout << "  1.1 -   ,     ,     ." << endl;
                    short int add_elem = first_full_square(A,N); //    
                    if (add_elem == -1) // ,   
                    {

                        cout << "***      ! ***" << endl;
                        cout << " ,   : ";
                        cin >> add_elem; //  ,    
                    }

                    short int zadannaya_cifra;
                    cout << "  : ";
                    cin >> zadannaya_cifra; //   
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "  : ";
                        cin >> zadannaya_cifra;
                    }

                    cout << "    : ";
                    print_array(A,N);

                    int N_temp = insert_future_size_of_array(A,N,zadannaya_cifra); //      
                    if (N_temp == N)
                    {
                        cout << "   ,      ,     ." << endl;
                    }
                    else
                    {
                        temp = new short int[N_temp]; //     temp
                        insert_after_var(A,temp,N,add_elem,zadannaya_cifra); //     A   Temp     
                        delete[] A; //     A
                        N = N_temp; //   N  A
                        A = new short int[N]; //       A
                        Copy(temp,A,N); //     temp   A
                        delete[] temp; //     temp
                        cout << "    : ";
                        print_array(A,N);
                    }
                    cout << " 1.1 !" << endl;
                    break;
                }
                case 2:
                {
                    cout << "  1.2 -   ,     ,     ." << endl;
                    short int add_elem = first_full_square(A,N); //    
                    if (add_elem == -1) // ,   
                    {

                        cout << "***      ! ***" << endl;
                        cout << " ,   : ";
                        cin >> add_elem; //  ,    
                    }

                    short int zadannaya_cifra;
                    cout << "  : ";
                    cin >> zadannaya_cifra; //   
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "  : ";
                        cin >> zadannaya_cifra;
                    }

                    cout << "    : ";
                    print_array(A,N);

                    int N_temp = insert_future_size_of_array(A,N,zadannaya_cifra); //      
                    if (N_temp == N)
                    {
                        cout << "   ,      ,     ." << endl;
                    }
                    else
                    {
                        temp = new short int[N_temp]; //     temp
                        insert_before_var(A,temp,N,add_elem,zadannaya_cifra); //     A   Temp     
                        delete[] A; //     A
                        N = N_temp; //   N  A
                        A = new short int[N]; //       A
                        Copy(temp,A,N); //     temp   A
                        delete[] temp; //     temp
                        cout << "    : ";
                        print_array(A,N);
                    }
                    cout << " 1.2 !" << endl;
                    break;
                }
                case 3:
                {
                    cout << "    : ";
                    print_array(A,N);
                    int N_temp = delete_future_size_of_array(A,N); //       
                    if (N_temp == N)
                    {
                        cout << "    ,       ." << endl;
                    }
                    else
                    {
                        temp = new short int[N_temp]; //     temp
                        delete_fibonachi(A,temp,N); //        temp,   
                        delete[] A; //     A
                        N = N_temp; //   N  A
                        A = new short int[N]; //       A
                        Copy(temp,A,N); //     temp   A
                        delete[] temp; //     temp
                        cout << "    : ";
                        print_array(A,N);
                    }
                    cout << " 2 !" << endl;
                    break;
                }
            }
        }
    }
    if (N == 0) cout << "      .";
    else cout << "     .";
    return 0;

}
