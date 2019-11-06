#include <iostream>

using namespace std;

double get_temp_mix (double temp1, double weight1, double temp2, double weight2)
{
    return (temp1 * weight1 + temp2 * weight2) / (weight1 + weight2); // температура смеси
}

int main()
{
    system("chcp 1251 > nul");
    int env_temp = 20; // температура окружающей среды
    double temp_voda1 = 100; // кипяток
    double temp_voda2 = 10; // холодная вода
    double weight_voda1 = 0.250; // 250 грамм горячей воды
    double weight_voda2 = 0.100; // 100 грамм холодной воды
    double k = 0.1; // коэф. остывания

    cout << "Температура окружающей среды: " << env_temp << endl;
    cout << "Температура первой жидкости: " << temp_voda1 << endl;
    cout << "Температура второй жидкости: " << temp_voda2 << endl;
    cout << "Вес первой жидкости: " << weight_voda1 << endl;
    cout << "Вес второй жидкости: " << weight_voda2 << endl;
    cout << " Что остынет быстрее?" << endl << " Если сразу влить в первую жидкость вторую или дать жидкости немного остыть, а затем налить вторую?" << endl;
    cout << "Время: 15 минут" << endl;
    cout << "Коэффициент остывания: " << k << endl;
    cout << "Для первого случая - ";
    cout << "Вес смеси: " << (weight_voda1 + weight_voda2);
    double temp_mix = get_temp_mix(temp_voda1, weight_voda1, temp_voda2, weight_voda2);
    cout << " - Температура смеси: " << temp_mix << endl;
    for (int i = 0; i < 15; ++i)
    {
        double d = temp_voda1 - env_temp; // дельта температур
        temp_voda1 -= d * k;

        d = temp_mix - env_temp;
        temp_mix -= d * k;
    }
    cout << "Первый случай: " << temp_mix << endl;
    temp_mix = get_temp_mix(temp_voda1,weight_voda1,temp_voda2,weight_voda2);
    cout << "Второй случай: " << temp_mix << endl;

    return 0;
}
