#include <iostream>
#include <array>
#include <memory>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <stack>

using namespace std;

struct cmd16_struct {
    uint16_t cop : 7;
    uint16_t t : 1;
    uint16_t r1 : 4;
    uint16_t r2 : 4;
};

union shortword
{
    uint8_t b[2];
    cmd16_struct c;
};

int main ()
{
    // проинициализировали как надо в трансляторе
    shortword command{};
    command.c.cop = uint8_t(2);
    command.c.t = 0;
    command.c.r1 = uint8_t(1);
    command.c.r2 = uint8_t(3);
    // сохранили в бинарный вид
    uint8_t binary[2];
    binary[0] = command.b[0];
    binary[1] = command.b[1];
    // типа записали этот бинари побайтово так же в файл через обычный >> (в режиме ios::binary)
    // типа прочитал бинари в интерпретаторе
    uint8_t binaryIn[2];
    binaryIn[0] = binary[0];
    binaryIn[1] = binary[1];
    // создал команду в ВМ
    shortword commanda2{};
    commanda2.b[0] = binaryIn[0];
    commanda2.b[1] = binaryIn[1];
    return 0;
}
