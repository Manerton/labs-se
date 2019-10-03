#include <iostream>
#include "bitstring.h"
#include <cassert>

using namespace std;

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №2. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    cout << "Условие: Создать класс BitString для работы с 128-битовыми строками."
            "Битовая строка должна быть представлена двумя полями типа unsigned long long. "
            "Должны быть реализованы все традиционные операции для работы с битами: and, or, xor, not. "
            "Реализовать сдвиг влево shiftLeft и сдвиг вправо shiftRight на заданное количество битов. "
            "Реализовать операцию вычисления количества единичных битов, операции сравнения по количеству единичных битов. "
            "Реализовать операцию проверки включения." << endl;
    try // ищем исключения, которые выбрасываются в блоке try и отправляем их для обработки в блок(и) catch
    {
        cout << "\n0. Создание битовой строки BS0 без инициализации (конструктор по умолчанию, т.е поля равны 0 и 0 соответственно) и последующий вывод строки BS0." << endl;
        BitString BS0;
        cout << BS0;
        cout << "\n1. Создание строки BS1, инициализация строкой \"00100110\" и последующий вывод строки BS1." << endl;
        BitString BS1("00100110");
        cout << BS1;
        cout << "\nРазмер класса в байтах: "; // в режиме pragma pack (1) будет такой же размер, так как все поля класса одного типа uint64_t
        cout << sizeof(BS1) << endl;
        cout << "\n2. Создание строки BS2, инициализация строкой введенной с клавиатуры." << endl;
        BitString BS2;
        cin >> BS2;
        cout << BS2;
        cout << "\n3. Создание строки BS3, инициализация объектом BS2 и последующий вывод BS3." << endl;
        BitString BS3 = BS2;
        cout << "BS3: " << BS3;
        BitString testBS1("110101");
        BitString testBS2("001001");
        BitString testBS3("100110");
        BitString testBS4("11010111100011101011110001011010110001100010000000000000000000011010111100011101011110001011010110001100010000000000000000000011");

        assert((BS1 & testBS1).toString == "100100");
        assert((BS1 & testBS2).toString == "0");
        assert((BS1 & testBS3).toString == "100110");
        assert((BS1 & testBS4).toString == "10");

        assert((BS1 | testBS1).toString == "110111");
        assert((BS1 | testBS2).toString == "101111");
        assert((BS1 | testBS3).toString == "100110");
        assert((BS1 | testBS4).toString == "11010111100011101011110001011010110001100010000000000000000000011010111100011101011110001011010110001100010000000000000000100111");

        assert((BS1 ^ testBS1).toString == "10011");
        assert((BS1 ^ testBS2).toString == "101111");
        assert((BS1 ^ testBS3).toString == "0");
        assert((BS1 ^ testBS4).toString == "11010111100011101011110001011010110001100010000000000000000000011010111100011101011110001011010110001100010000000000000000100101");

        assert((~testBS1).toString == "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111001010");
        assert((~testBS2).toString == "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110110");
        assert((~testBS3).toString == "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011001");
        assert((~testBS4).toString == "00101000011100010100001110100101001110011101111111111111111111100101000011100010100001110100101001110011101111111111111111111100");

        assert((testBS1 << 3).toString == "110101000");
        assert((testBS2 << 3).toString == "1001000");
        assert((testBS3 << 3).toString == "100110000");
        assert((testBS4 << 3).toString == "10111100011101011110001011010110001100010000000000000000000011010111100011101011110001011010110001100010000000000000000000011000");

        assert((testBS1 >> 4).toString == "0000110101");
        assert((testBS2 >> 4).toString == "00001001");
        assert((testBS3 >> 4).toString == "0000100110");
        assert((testBS4 >> 4).toString == "1101011110001110101111000101101011000110001000000000000000000001101011110001110101111000101101011000110001000000000000000000");

        cout << "\n4. Количество единичных битов в BS1: " << (BS1.count_of_SingleBit());

        assert((BS1 < testBS1) == 1);
        assert((BS1 >= testBS1) == 0);
        assert((BS1 > testBS1) == 0);
        assert((BS1 <= testBS1) == 1);
        assert((BS1 == testBS1) == 0);
        assert((BS1 != testBS1) == 1);

        assert((BS1 < testBS2) == 0);
        assert((BS1 >= testBS2) == 1);
        assert((BS1 > testBS2) == 1);
        assert((BS1 <= testBS2) == 0);
        assert((BS1 == testBS2) == 0);
        assert((BS1 != testBS2) == 1);

        assert((BS1 < testBS3) == 0);
        assert((BS1 >= testBS3) == 1);
        assert((BS1 > testBS3) == 0);
        assert((BS1 <= testBS3) == 1);
        assert((BS1 == testBS3) == 1);
        assert((BS1 != testBS3) == 0);

        assert((BS1 < testBS4) == 1);
        assert((BS1 >= testBS4) == 0);
        assert((BS1 > testBS4) == 0);
        assert((BS1 <= testBS4) == 1);
        assert((BS1 == testBS4) == 0);
        assert((BS1 != testBS4) == 1);


        assert(testBS1.count_of_SingleBit() == 4);
        assert(testBS2.count_of_SingleBit() == 2);
        assert(testBS3.count_of_SingleBit() == 3);
        assert(testBS4.count_of_SingleBit() == 50);

        cout << "\n16. Операция (BS1 < BS2) :" << (BS1<BS2);
        cout << "\n17. Операция (BS1 >= BS2) :" << (BS1>=BS2);
        cout << "\n18. Операция (BS1 > BS2) :" << (BS1>BS2);
        cout << "\n19. Операция (BS1 <= BS2) :" << (BS1<=BS2);
        cout << "\n20. Операция (BS1 == BS2) :" << (BS1==BS2);
        cout << "\n21. Операция (BS1 != BS2) :" << (BS1!=BS2);
        cout << "\n22. Операция проверки включения BS1 в BS2:" << (is_included(BS1,BS2));
        cout << "\n23. Операция проверки включения BS2 в BS1:" << (is_included(BS2,BS1));

    }
    catch (BitString::negative_int_exception) // обработка исключений типа const char*
    {
        cerr << "Ошибка: введенное число не может быть отрицательным." << endl;
    }
    catch (BitString::not_binary_exception) // обработка исключений типа const char*
    {
        cerr << "Ошибка: введенное число не является двоичным." << endl;
    }
    catch (BitString::overflow_exception) // обработка исключений типа const char*
    {
        cerr << "Ошибка: битовая строка не может быть больше чем 128 бита." << endl;
    }
    return 0;
}

