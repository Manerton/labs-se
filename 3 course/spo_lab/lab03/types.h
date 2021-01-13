#ifndef TYPES_H
#define TYPES_H
#include <cinttypes>
#include <array>
#include <memory>
// предварительно объявляем для commandTableElem
class Command;

// характеристики архитектуры, типы и их размеры
namespace VM_types
{
    using byte_t = uint8_t;
    using address_t = uint16_t;

    static constexpr uint8_t cmd_length = 3;    // -- длина команды - 3 байта, увеличиваем IP на эту величину -- //
    static constexpr uint8_t data_length = 4;   // -- 4 байта для данных (целые, дробные) -- //
    static constexpr uint8_t BPCount = 8;       // количество точек прерываний


    union cmd_t   // -- команда 3 байта -- //
    {
        #pragma pack(push, 1)
        struct cmd24_struct {
            uint8_t cop : 8;
            address_t address : 16;  // -- адрес, константа, смещение -- //
        };
        #pragma pack(pop)
        cmd24_struct c{0,0};
        std::array<byte_t,3> b;  // -- 3 байта -- //
    };

    union data_t {  // -- тип данных (размер 4 байта, 32-бита) -- //
        uint32_t u = 0; // -- беззнаковое число -- //
        int32_t i;  // -- знаковое число -- //
        float f;    // -- дробное число -- //
        std::array<byte_t,4> b;     // -- слово, 4 байта -- //
    };

    // для отладки
    // режим доступа для точки прерывания и команд
    enum class AccessMode: byte_t
    {
        None = 0,   // точка выключена (или команда никуда не обращается)
        Exec,           // по адресу выполняющейся команды (это для точки прерывания, команды не вызывают другие команды)
        Read,           // по обращению к адресу памяти (чтение)
        Write           // по записи в ячейку памяти
    };
    struct Breakpoint {
        AccessMode mode = AccessMode::None;
        address_t address = 0;
    };

    // элемент таблицы команд в CPU
    struct commandTableElem
    {
        AccessMode mode = AccessMode::None;
        std::unique_ptr<Command> cmd_ptr = nullptr;
    };
}
#endif // TYPES_H
