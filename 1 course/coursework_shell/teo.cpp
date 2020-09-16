#include <iostream>
#include <fstream>
#include <conio.h> // ��� getch()
#include <sstream> // �������� ��������� �����
#include "color.h" // ������� �����
#include "teo.h"

//������ ������ �� �����

void ShowTeoHelp()
{
    SetColor(Blue,White);
    cout << "�� ������� ����� ������ ������: \n" << endl;
    SetColor(Black,White);
    cout << "������� '->', ��� �������� � ��������� ��������" << endl;
    cout << "������� '<-', ��� �������� � ���������� ��������" << endl;
    cout << "������� '\\/', ���� �� ������ ��������� ������ ������ � ��������� � ����\n" << endl;
    system("pause");
    system("cls");
}

void creation_text (text &text){
    ifstream input("files\\teo.dat");
    if(input)
    {
        string temp_string; // ������� ������
        string all_string; // ���� ����� � ����� ������
        page temp_page;
        while (getline(input, temp_string)) // ������������� ����� �� �������������� ����� � ��������� ��� � ����� ������
        {
            if (!input.eof()) temp_string += "\n"; // �������� \n ��� ��� Getline ��� �� ��������
            for (char& i : temp_string) i ^= 83; // �������������
            all_string += temp_string;
        }

        stringstream ss(all_string); // ������ ��������� ����� �� all_string
        while (getline(ss, temp_string)) // ���� ����� ������ �� ������ (��� ��� � ����� ������ ���� ������� \n)
        {
            if (temp_string.find("[next_page]") != string::npos)
            {
                text.push_back(temp_page);
                temp_page.clear();
                //continue;
            } else temp_page.push_back(temp_string);
        }
    } else cout <<"���� �� ���������!" << endl;

}

// ����� ����� �������� ������ � ���������� ������ ����
void one_page_display (const page &onepage)
{
    string blank(26, ' ');
    size_t N = onepage.size();
    cout << endl;
    for (size_t i = 0; i < N; i++)
    {
        cout << blank;

        size_t pos = onepage[i].find("[color]"); // ��� ������� ���������� ����

        if (pos != string::npos) // ���� ������ ��� [color]...[/color] ������� �������� ������ �����������
        {
            size_t strlen = onepage[i].length();
            for (size_t j = 0; j < strlen; j++)
            {
                if (j == pos)
                {
                    j += 7; // ������������ ���, ����� ��� �� �������
                    pos = onepage[i].find("[/color]", pos); // ��� ������� ������������ ����
                    SetColor(Green, White); // ������� ����� ����
                    while (j < pos && j < strlen) // ������ ���������� ������ �����
                    {
                        cout << onepage[i][j];
                        j++;
                    }
                    SetColor(Black, White); // ������� ����������� ����
                    j += 8; // ������������ ���, ����� ��� �� �������
                    pos = onepage[i].find("[color]", pos); // ��� ������� ���������� ������������ ���� � ������
                }
                cout << onepage[i][j];
            }
        } else cout << onepage[i]; // ����� ������ ����� ������

        cout << endl;
    }
    cout << endl;
}

// ����� �������
void teo_display (text TEXT){
    string blank(26, ' ');
    string blank2(54, ' ');
    string lower(59, ' ');
    ShowTeoHelp();
    int page_num = 0, ch = 0;
    int size = TEXT.size();
    while (page_num != size && ch == 0)
    {
        cout << blank;
        SetColor(White,Green);
        cout << blank2 <<"["<< page_num+1 << "/" << size << "]" << endl;
        SetColor(Black,White);
        one_page_display(TEXT[page_num]);
        cout << blank;
        SetColor(White,Green);
        if (page_num != 0) cout << "<-";
        else cout << "  ";
        cout << blank << " \\/" << blank;
        if (page_num != size-1) cout << "->";
        else cout << "  ";
        SetColor(Black,White);

        switch(getch())
        {
        case 77:
            if (page_num != size-1) page_num++;
            break;
        case 75:
            if (page_num != 0) page_num--;
            break;
        case 80:
            ch = 1;
            break;
        }
        system("cls");
    }
}

// ����������
void crypt (string filename)
{
    ifstream in("files\\" + filename + ".txt");
    if(in)
    {
        cout << "������ ���� " << filename + ".txt..." << endl;
        ofstream out("files\\" + filename + ".dat", ios::binary);
        string temp;
        while (getline(in, temp)) // ������ �� ������ � ����������������� ���������
        {
            if (!in.eof()) temp += "\r\n"; // ���������� � ������ ������� ��������������
            for (char &i : temp) i ^= 83; // � ����� ��� ������������� ������
            out.write(temp.data(), temp.size());
        }
        cout << "���� ��� ������� ���������� � �������� ��� ������ " << filename + ".dat" << endl;
    } else cout << "���� �� ���������!" << endl;
}

// �����������
void decrypt (string filename)
{
    ifstream in("files\\" + filename + ".dat");

    if(in)
    {
        cout << "������������� ���� " << filename + ".dat..." << endl;
        ofstream out("files\\" + filename + ".txt", ios::binary);
        string temp;

        while (getline(in, temp)) // ������ ������ (��� ����� ���� ������������ �����)
        {
            if (!in.eof()) temp += "\n"; // ��������� ������� ������ \n � ����, ��� ��� getline �������� ������ ��������� \n
            for (char &i : temp) i ^= 83; // ��������������
            out.write(temp.data(), temp.size());
        }
        cout << "���� ��� ������� ����������� � �������� ��� ������ " << filename + ".txt" << endl;
    } else cout << "���� �� ���������!" << endl;
}
