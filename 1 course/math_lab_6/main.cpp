#include <iostream>
#include <vector>
#define op_or 1
#define op_and 2
#define op_equ 3
#define op_imp 4
#define op_xor 5

using namespace std;

struct element
{
    short int val;
    bool inv = false;
};

typedef vector<bool> row;
typedef vector<element> row_sdnf;
typedef vector<row> matrix;
typedef vector<row_sdnf> matrix_SDNF;

bool s_op (bool a, bool b, int op_index)
{
    switch (op_index)
    {
    case op_or:
        return a || b;
        break;
    case op_and:
        return a && b;
        break;
    case op_equ:
        return a == b;
        break;
    case op_imp:
        return a <= b;
        break;
    case op_xor:
        return a ^ b;
        break;
    }
    return false;
}
bool operation (bool x, bool y, bool z, vector<int> oper)
{
    //x Л1 ((y Л2 z) Л3 (x Л4 y))
    bool l2 = s_op(y,z,oper[1]);
    cout << l2 << "  | ";
    bool l4 = s_op(x,y,oper[3]);
    cout << l4 << "  | ";
    bool l3 = s_op(l2,l4,oper[2]);
    cout << l3 << "  | ";
    return (s_op(x,l3,oper[0]));

}

void SDNF (const matrix &F, matrix_SDNF &F_out)
{

    int N = F.size();
    for (int i = 0; i < N; i++)
    {
        if (F[i][3] == 1)
        {
            row_sdnf Row;
            Row.resize(3);

            Row[0].val = F[i][0]; // записываю матрицу SDNF
            if (F[i][0] == 0) Row[0].inv = true;
            Row[1].val = F[i][1];
            if (F[i][1] == 0) Row[1].inv = true;
            Row[2].val = F[i][2];
            if (F[i][2] == 0) Row[2].inv = true;
            F_out.push_back(Row);
        }
    }
}

void SDNF_display (const matrix_SDNF &F)
{
    cout << "Строим СДНФ: " << endl;
    int N = F.size();
    cout << "F(x, y, z) = ";
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (count == 0) cout << "(";
        else cout << " + (";

        if (F[i][0].inv == true) cout << "¬x • ";
        else cout << "x • ";

        if (F[i][1].inv == true) cout << "¬y • ";
        else cout << "y • ";

        if (F[i][2].inv == true) cout << "¬z";
        else cout << "z";
        cout << ")";
        count++;

    }
}

void SKNF (const matrix &F)
{
    cout << "Вычисляем СКНФ: " << endl;
    int N = F.size();
    cout << "F(x, y, z) = ";
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (F[i][3] == 0)
        {
            if (count == 0) cout << "(";
            else cout << " • (";
            if (F[i][0] == 1) cout << "¬x + ";
            else cout << "x + ";
            if (F[i][1] == 1) cout << "¬y + ";
            else cout << "y + ";
            if (F[i][2] == 1) cout << "¬z";
            else cout << "z";
            cout << ")";
            count++;
        }

    }
}

void Zegalkin (const matrix &F, matrix &F_out) // где F - таблица истинности, F_out - матрица жегалкина
{
    int N = F.size();
    row Row(N,0);
    for (int i = 0; i < N; i++)
    {
        Row[i] = F[i][3];
    }
    F_out.push_back(Row);
    int size = N-1;
    for (int j = 1; j < N; j++)
    {
        row Row2(size, 0);
        int M = Row.size();
        for (int i = 0; i < M; i++)
        {
            Row2[i] = Row[i] ^ Row[i+1];
        }
        F_out.push_back(Row2);
        Row = Row2;
        size--;
    }
}

void Zegalkin_display (const matrix &F, const matrix &F_Z, bool &L)
{
    L = true;
    cout << "Строим полином Жегалкина: " << endl;
    int N = F_Z.size();
    cout << "F(x, y, z) = ";
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (F_Z[i][0] == 1)
        {
            if (count == 0) cout << "(";
            else cout << " ^ (";
            if (F[i][0] == 1) cout << "x";
            if (F[i][1] == 1) cout << "y";
            if (F[i][2] == 1) cout << "z";
            if (F[i][0] == 0 && F[i][1] == 0 && F[i][2] == 0) cout << "1";
            if ((F[i][0] == 1 && F[i][1] == 1) || (F[i][0] == 1 && F[i][2] == 1) || (F[i][1] == 1 && F[i][2] == 1)) L = false;
            cout << ")";
            count++;
        }
    }
}


vector<int> choice_oper()
{
    vector<int>oper;
    int ch;
    cout << "Задана булева функция F вида: x Л1 ((y Л2 z) Л3 (x Л4 y))" << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "Выберите логическую операцию Л" << i+1 << ": " << endl;
        cout << "1. Дизъюкция." << endl;
        cout << "2. Конъюкция." << endl;
        cout << "3. Эквивалентность." << endl;
        cout << "4. Импликация." << endl;
        cout << "5. Сложение по модулю 2." << endl;
        cout << "Ввод: ";
        cin >> ch;
        while (ch < 1 || ch > 5)
        {
            cout << "error!" << endl;
            cout << "Выберите логическую операцию: " << endl;
            cin >> ch;
        }
        oper.push_back(ch);
    }
    return oper;
}

bool self_dual (const matrix &F)
{
    int N = F.size();
    int size = N / 2;
    for (int i = 0; i < size; i++)
    {
        if (F[i][3] == F[N-i-1][3]) return false;
    }
    return true;
}

bool proverka_na_mono (const matrix &F)
{
    int N = F.size();
    for (int i=0; i<N-1; i++)
    {
        for (int j=i+1; j < N; j++)
        {
            if (F[i][0] <= F[j][0] && F[i][1] <= F[j][1] && F[i][2] <= F[j][2])
            {
                if (F[i][3] > F[j][3])
                    return false;
            }
        }

    }
    return true;
}

int main()
{
    system("chcp 1251 > nul");

    int ch = 1;
    while (ch != 0)
    {

        vector<int> oper = choice_oper();

        matrix F;
        cout << "Задана булева функция F вида: x Л1 ((y Л2 z) Л3 (x Л4 y))" << endl;
        cout << "x | y | z | Л2 | Л4 | Л3 | F " << endl;
        bool x (0), y (0), z(0);
        bool result;
        for (int i=0; i<2; i++)
        {
            y = 0;
            for (int j=0; j<2; j++)
            {
                z = 0;
                for (int k=0; k<2; k++)
                {
                    row Row(4,0); // записываем состояние таблицы истинности
                    Row[0] = x; // записываем x, y, z
                    Row[1] = y;
                    Row[2] = z;

                    cout << x << " | " << y << " | " << z << " | ";
                    result = operation(x,y,z,oper);
                    Row[3] = result; // и значение функции F при этих значения x, y, z
                    cout << result << endl; // выводим значение F на экран
                    F.push_back(Row);
                    z = 1;
                }
                y = 1;
            }
            x = 1;
        }
        matrix_SDNF F_SDNF;
        SDNF(F, F_SDNF);
        SDNF_display(F_SDNF);
        cout << "\n\n";
        SKNF(F);
        cout << "\n\n";
        matrix F_Z;
        Zegalkin(F, F_Z);
        bool L;
        Zegalkin_display(F, F_Z, L);

        cout << "\n\nT0 | T1 | S | M | L " << endl;
        bool T0 = 1 - F[0][3];
        bool T1 = F[7][3];
        bool S = self_dual(F);
        bool M = proverka_na_mono(F);
        cout << T0 << "  | " << T1 << "  | " << S << " | " << M << " | " << L << endl;
        cout << " \n 0. Выход \n 1. Пересгенерировать" << endl;
        do
        {
            cout << " \nВвод: ";
            cin >> ch;
        }
        while ((ch < 0) || (ch > 1));
    }
    return 0;
}
