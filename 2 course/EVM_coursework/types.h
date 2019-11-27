#ifndef TYPES_H
#define TYPES_H

#include <cinttypes>

union word_t // для того, чтобы легко разместить две 16-битные команды в 1 слове
{
    struct half_words_t {
        uint16_t low;  // младшее полуслово
        uint16_t high; // старшее полуслово
    };

    uint32_t w;
    half_words_t hw;
};

union cmd16_t {
    struct cmd16_struct {
        uint8_t cop : 8;
        uint8_t r1 : 4;
        uint8_t r2 : 4;
    };

    cmd16_struct cmd;
    uint16_t hw;
};

union cmd32_t {
    struct cmd32_struct {
        uint8_t cop : 8;
        uint8_t r1 : 4;
        uint8_t r2 : 4;
        uint16_t address : 16;
    };

    cmd32_struct cmd;
    uint32_t w;
};

union data_t {
    int32_t i;
    uint32_t u;
    float f;
    cmd32_t c;
};

#endif // TYPES_H
