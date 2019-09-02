#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "struct.h"

using namespace std;

void Read(vector<NOTEBOOK> &X)
{
    ifstream input("note.txt");
    if (input)
    {
        while (!input.eof())
        {
            NOTEBOOK onenote;
            input >> onenote.model;
            input >> onenote.price;
            input >> onenote.w;
            string temp;
            input >> temp; // читаю габариты
            onenote.size.x = stof(temp.substr(0, 4));
            onenote.size.y = stof(temp.substr(5, 4));
            onenote.size.z = stof(temp.substr(10, 4));
            input >> onenote.cpu;
            input >> onenote.ram;
            X.push_back(onenote);
            getline(input,temp);
        }
        input.close();
    } else cout << "Не удалось открыть файл note.txt" << endl;

}

void CreateEmptyBinFile(string filename)
{
    ofstream createBin(filename, ios::binary|ios::app);
    if(createBin)
    {
        createBin.close();
    } else cout << "Не удалось создать файл " << filename;
}

void ModifyBinFile(const vector<NOTEBOOK> &X, string filename)
{
    short int N = X.size(); // кол-во записей в структуре
    short int New_N = N;
    fstream toBin(filename, ios::binary|ios::in|ios::out);
    if(toBin)
    {
        short int Old_N(0); // количество записей в файле
        toBin.read((char*)&Old_N, sizeof(short int)); // читаю количество записей
        toBin.clear(); // обнуляю флаг, на случай если файл пуст
        New_N += Old_N; // вычисляю новое кол-во записей
        toBin.seekp(0); // перезаписываю два байта в начале файла (количество записей)
        toBin.write((char*)&New_N, sizeof(short int));
        toBin.seekp(0, ios::end); // пишу в конец файла структуру
        for(int i=0; i<N; i++)
        {
            toBin.write((char*)&X[i], sizeof(NOTEBOOK));
        }
        toBin.close();
    } else cout << "Не удалось открыть файл " << filename;
}

void FromBinFile(vector<NOTEBOOK>&X, string filename)
{
    short int N;
    ifstream fromBin(filename, ios::binary);
    if(fromBin)
    {
        fromBin.read((char*)&N, sizeof(short int));
        for(int i=0; i<N; i++)
        {
            NOTEBOOK onenote;
            fromBin.read((char*)&onenote, sizeof(NOTEBOOK));
            X.push_back(onenote);
        }
        fromBin.close();
    } else cout << "Не удалось открыть файл " << filename;
}

void sort_arr (vector<NOTEBOOK> &X)
{
    int N = X.size();
    for (int i = 0; i != N-1; i++)
    {
        for (int j = i+1; j != N; j++)
        {
           if (X[i].ram < X[j].ram) swap(X[i],X[j]);  // сортирую
        }
    }
}

void notebooks_with_40_and_more_ram (const vector<NOTEBOOK> &old, vector<NOTEBOOK> &newone)
{
    int N = old.size();
    for (int i = 0; i < N; i++)
    {
        if (old[i].ram >= 40) newone.push_back(old[i]);  // сортирую
    }
}

void Display(const vector<NOTEBOOK> &X)
{
    int N = X.size();
    cout << "=================================================\n";
    for (int i=0; i<N; i++)
    {
        cout << X[i].model << " | ";
        cout << "Цена: " << X[i].price << " | ";
        cout << "Вес: " << X[i].w << " | ";
        cout << "Габариты: " << X[i].size.x << "x" << X[i].size.y << "x" << X[i].size.z << " | ";
        cout << "Частота ЦПУ: " << X[i].cpu << " | ";
        cout << "Объем ОЗУ: " << X[i].ram << endl;
    }
    cout << "=================================================\n";
}
