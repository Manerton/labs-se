// ver 1.1 - 08.04.19 - ����� ��������
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

struct Node
{
    short int elem;
    Node *next;
};
typedef Node *PNode;
PNode Head;

 // �������� ������ ������

//������� ����� ���� � ������ � ���������� ��� �����
PNode CreateNode (short int NewElem)
{
    PNode NewNode = new Node;
    NewNode->elem = NewElem;
    NewNode->next = NULL;
    return NewNode;
}
// �������� � ������ ������
void AddFirst(PNode &Head, PNode NewNode)
{
    NewNode->next = Head; // ������, ����� � NewNode ������ ���� �� ���������� ������ ������
    Head = NewNode; // ������ ����� ������� NewNode
}
void AddAfter (PNode p, PNode NewNode)
{
    NewNode->next = p->next;
    p->next = NewNode;
}
// �������� � ����� ������
void AddLast(PNode &Head, PNode NewNode)
{
    PNode q = Head;
    if (Head == NULL) // ������ ����
    {
        AddFirst(Head, NewNode);
        return;
    }
    while (q->next) q = q->next;
    AddAfter(q, NewNode);
}

// �������� �� �������
bool empty_list (PNode &Head)
{
    if (Head == NULL) return true;
    else return false;
}
// ����� �����
void print_list (PNode &Head)
{
    PNode q = Head;
    if (empty_list(Head)) cout << "���� ����." << endl;
    else
    {
        while (q)
        {
            cout << q->elem << " ";
            q = q->next;
        }
        cout << endl;
    }
}
void filling_list_from_keyboard (PNode &Head, int N)
{
    cout << "*** �������������� ���������� ������������ ������ ���������� � ���������� ***" << endl;
    cout << "������� �������� ��������� ������������ ������: " << endl;
    int chislo;
    PNode NewNode;
    for (int i = 0; i < N; i++)
    {
        cout << "��� " << i+1 << "-��: ";
        cin >> chislo;
        NewNode = CreateNode(chislo);
        AddLast(Head, NewNode);
    }
}

void filling_list_random (PNode &Head, int N, int min, int max)
{
    cout << "*** �������������� ���������� ������������ ������ ���������� ������ ������� ***" << endl;
    int chislo;
    PNode NewNode;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        NewNode = CreateNode(chislo);
        AddLast(Head, NewNode);
    }
}

void filling_list_from_file (PNode &Head, int N, string filename)
{
    cout << "*** �������������� ���������� ������������ ������ �� ����� " << filename << " ***" << endl;
    int chislo;
    PNode NewNode;
    ifstream input(filename);
    if (input) // �������� ��������
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            NewNode = CreateNode(chislo);
            AddLast(Head, NewNode);
        }
        input.close();
    } else cout << endl << "error! �� ������� ������� ���� " << filename << endl;
}

void diapazon_rand (int &min, int& max)
{
        cout << "������� �������� ������������� ��������� �����." << endl;
        cout << "������ �������: ";
        cin >> min;
        cout << "������� �������: ";
        cin >> max;
        while (min >= max)
        {
            cout << "error!" << endl;
            cout << "������� �������: ";
            cin >> max;
        }
}

void sposob_zapolneniya (PNode &Head, int N)
{
    int choice;
    cout << "*** ����� ������� ���������� ***" << endl;
    cout << "1. ���������� � ����������." << endl;
    cout << "2. ���������� ���������� �������." << endl;
    cout << "3. ���������� �� �����." << endl;
    cout << "�������� ������ ����������: ";
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "error!" << endl;
        cout << "�������� ������ ����������: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        filling_list_from_keyboard(Head, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_list_random(Head, N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "������� �������� �����: ";
        cin >> filename;
        filling_list_from_file(Head, N,filename);
        break;
    }
}

//�������� �� ������ ������� (���� ������ double ����� ����� ����� ������������ � int, �� ����� - ������ �������)
bool full_square (short int chislo)
{
    double sqrt_chislo = sqrt(chislo);
    if (chislo >= 0 && sqrt_chislo == static_cast<short int> (sqrt_chislo)) return true;
    else return false;
}

void display_menu ()
{
    cout << "*** ���� ***" << endl;
    cout << "1. ������� 1.1 - �������� ������ �������, ���������� ������ ��������� ����� �����, � ������� ��� �������� �����." << endl;
    cout << "2. ������� 1.2 - �������� ������ �������, ���������� ������ ��������� ����� ������, � ������� ��� �������� �����." << endl;
    cout << "3. ������� 2 - ������� ��� ����� ���������." << endl;
    cout << "4. ��������� ������ ���������." << endl;
}
int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "������������ ������ �6 ������� 13.\n�����: ������� ������. �����-11.\n" << endl;
    // ������ ����� ������� ����
    display_menu();
    PNode Head = NULL;
    // ���� ������� ������
    int N;
    cout << endl << "������� ������ ������������ ������: ";
    cin >> N;
    // �������� �� ������
    while (N < 1)
    {
        cout << "error!" << endl;
        cout << "������� ������ ������������ ������: ";
        cin >> N;
    }
    // ����� �����

    // ������ (������, ������) ����������� ���� ���
    sposob_zapolneniya(Head, N);
    cout << "���������� ����������� ������: ";
    print_list(Head);

    return 0;

}
