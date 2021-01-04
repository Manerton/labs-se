#ifndef TYPES_H
#define TYPES_H
#include <cinttypes>

// характеристики архитектуры, типы и их размеры
namespace VM_types
{
    static constexpr uint8_t cmd_length = 3;    // -- длина команды - 3 байта, увеличиваем IP на эту величину -- //
    static constexpr uint8_t data_length = 4;   // -- 4 байта для данных (целые, дробные) -- //
    using byte_t = uint8_t;

    union cmd_t   // -- команда 3 байта -- //
    {
        #pragma pack(push, 1)
        struct cmd24_struct {
            uint8_t cop : 8;
            uint16_t address : 16;  // -- адрес, константа, смещение -- //
        };
        #pragma pack(pop)
        cmd24_struct c{0,0};
        byte_t b[3];  // -- 3 байта -- //
    };

    union data_t {  // -- тип данных (размер 4 байта, 32-бита) -- //
        uint32_t u = 0; // -- беззнаковое число -- //
        int32_t i;  // -- знаковое число -- //
        float f;    // -- дробное число -- //
        byte_t b[4];     // -- слово, 4 байта -- //
    };
}
#endif // TYPES_H
