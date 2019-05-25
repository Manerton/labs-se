#include <iostream>
#include <vector>
#include "struct.h"
using namespace std;

void Read(vector<NOTEBOOK> &X);
void ModifyBinFile(const vector<NOTEBOOK> &X, string filename);
void FromBinFile(vector<NOTEBOOK> &X, string filename);
void CreateEmptyBinFile(string filename);
void Display(const vector<NOTEBOOK> &X);
void sort_arr (vector<NOTEBOOK> &X);
void notebooks_with_40_and_more_ram (const vector<NOTEBOOK> &old, vector<NOTEBOOK> &newone);

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №11 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    vector<NOTEBOOK> V;
    Read(V);
    cout << "Полученный из файла вектор данных о ноутбуках: " << endl;
    Display(V);

    system("pause");
    system("cls");

    sort_arr(V);
    cout << "Отсортированный вектор данных о ноутбуках по убыванию объема ОЗУ: " << endl;
    Display(V);

    system("pause");
    system("cls");

    vector<NOTEBOOK> V_out;
    notebooks_with_40_and_more_ram (V, V_out);
    cout << "Ноутбуки с объемом ОЗУ не менее 40 МБ: " << endl;
    Display(V_out);

    system("pause");
    system("cls");

    CreateEmptyBinFile("output.bin");
    ModifyBinFile(V_out,"output.bin");
    cout << "Операция по редактированию бинарного файла завершена!" << endl;
    vector<NOTEBOOK> V_from_bin;
    FromBinFile(V_from_bin, "output.bin");
    cout << "Данные о ноутбуках, прочитанные из бинарного файла: " << endl;
    Display(V_from_bin);
    return 0;
}
