//  Ассемблерные вставки на GCC с флагом -masm=intel в .pro для синтаксиса Intel
//  (тоже самое можно и через команды .intel_syntax noprefix в начале asm вставки (и после кода, но внутри вставки, надо было вкл обратно AT&T .att_syntax noprefix)
//  использовать либо глобальные переменные, или локальные объявлять согласно AT&T syntax (или это GAS ассемблер, не знаю точно) после кода через : выходные операнды, .., .., : входные, .., ..
//  в квадратных скобках можно указать имя операнда входного / выходного
//  m - ограничитель, означает, что операнд передается через память, а всякие ir, r и так далее - это РОН
//  во 2 функции показал, как можно без \n обойтись




#include <stdio.h>
#include <iostream>

inline int cmp_asm(int a, int b, int c){
    asm( //ассемблерная функция
        "mov edx, %[a]\n" // помещение в регистр еdх значения переменной а
        "cmp edx, %[b]\n" // сравнение содержимого регистра edx и переменной b
        "ja m1\n" // условный переход
        "mov edx, %[b]\n" // помещение в регистр еdх значения переменной b
        "m1: cmp edx, %[c]\n" // сравнение содержимого регистра edx и переменной c
        "jna m2\n" // условный переход
        "mov %[c], edx\n" // помещение в переменную c содержимого регистра еdх
        "m2:\n"
        : [c] "+m" (c)
        : [a] "m" (a), [b] "m" (b)
        );
    return c;
}

inline int cmp_asm_2(int a, int b, int c){

    asm (
        R"(
        mov edx, %[a] # коммент
        cmp edx, %[b]
        ja m1
        mov edx, %[b]
        m1: cmp edx, %[c]
        jna m2
        mov %[c], edx
        m2:
        )"
        : [c] "+m" (c)
        : [a] "m" (a), [b] "m" (b)
        );
    return c;

}

int main()
{
    int k, m, n; //инициализация целочисленных переменных
    printf("Compare\n");
    printf("Please, input 1st number\n");
    scanf("%d", &k);
    printf("Please, input 2nd number\n");
    scanf("%d", &m);
    printf("Please, input 2nd number\n");
    scanf("%d", &n);
    std::cout << cmp_asm(k, m, n);
    scanf("%d", &n);
    return 0;
}





