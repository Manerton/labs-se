#include <iostream>
#include <cassert>
#include <bitset>

using namespace std;
/* 1. Используя операцию «логическое ИЛИ»,
      включите первый и третий биты числа */

/* 2. Написать программу компактного представления свойств окна
      (цвет, размер, наименование, меню, прозрачность в %)
      с нужным набором констант */

namespace WS
{

const uint8_t RED   = 0b001;
const uint8_t GREEN = 0b010;
const uint8_t BLUE  = 0b100;

const uint8_t SMALL  = 0b001;
const uint8_t MEDIUM = 0b010;
const uint8_t BIG    = 0b100;

const uint8_t NO_NAME     = 0b01;
const uint8_t CUSTOM_NAME = 0b10;

const uint8_t NO_MENU       = 0b001;
const uint8_t DEFAULT_MENU  = 0b010;
const uint8_t CUSTOM_MENU   = 0b100;

const uint8_t OPACITY_0      = 0b00001;
const uint8_t OPACITY_25     = 0b00010;
const uint8_t OPACITY_50     = 0b00100;
const uint8_t OPACITY_75     = 0b01000;
const uint8_t OPACITY_100    = 0b10000;

struct window_settings  // -- свойства окна, уместились в два байта -- //
{
    uint16_t color : 3;         // -- RED, GREEN, BLUE -- //
    uint16_t size : 3;          // -- SMALL, MEDIUM, BIG -- //
    uint16_t name : 2;          // -- NO_NAME, CUSTOM_NAME --//
    uint16_t menu_policy: 3;    // -- NO_MENU, DEFAULT_MENU, CUSTOM_MENU -- //
    uint16_t opacity : 5;       // -- OPACITY_0, OPACITY_25, OPACITY_50, OPACITY_75, OPACITY_100 -- //
};

}

uint32_t set_1_and_3_flags(uint32_t a)
{
    const uint32_t mask = 0b101; // -- 1 и 3 бит -- //
    a |= mask;
    return a;
}

inline uint32_t asm_set_1_and_3_flags(uint32_t a)
{
    const uint32_t mask = 0b101; // -- 1 и 3 бит -- //
    asm __volatile__ (
        R"(
        mov eax, %[a]    # поместим переменную a в регистр eax
        mov ebx, %[mask] # а маску в ebx
        or eax, ebx      # выполним операцию ИЛИ
        mov %[a], eax    # результат поместим в переменную a
        )"
        : [a] "+m" (a)   // -- выходные операнды -- //
        : [mask] "m" (mask) // -- входные операнды -- //
    );
    // -- "m" - означает, что операнд хранится в памяти (написав "r", можно указать, чтобы переменная автоматически поместилась в свободный регистр) -- //
    // -- "+" перед "m" - означает, что операнд a является не только выходным, но и входным -- //
    // -- если указать "=" вместо "+", то операнд будет только выходным (игнорируется значение переменной до асм вставки) -- //
    return a;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №3. Вариант 2.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    uint32_t a       = 0b11010000;
    uint32_t new_a   = 0b11010101; // -- должно стать так -- //
    a = set_1_and_3_flags(a);
    assert (a == new_a);

    a       = 0b00000000;
    new_a   = 0b00000101;
    a = set_1_and_3_flags(a);
    assert (a == new_a);

    a       = 0b11010000;
    new_a   = 0b11010101;
    a = asm_set_1_and_3_flags(a);
    assert (a == new_a);

    a       = 0b00000000;
    new_a   = 0b00000101;
    a = asm_set_1_and_3_flags(a);
    assert (a == new_a);

    // -- второе задание -- //
    WS::window_settings ws;
    ws.color = WS::RED | WS:: GREEN | WS::BLUE;
    ws.size = WS::MEDIUM;
    ws.name = WS::NO_NAME;
    ws.menu_policy = WS::DEFAULT_MENU;
    ws.opacity = WS::OPACITY_75;
    return 0;
}
