#include <iostream>
#include <algorithm>
#include <thread>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

using namespace std;
using namespace boost::interprocess;

/// Структура с счетчиками запуска программ
struct Counters
{
    uint16_t first = 0;     ///< Счетчик числа запусков первой программы (этой)
    uint16_t second = 0;    ///< Счетчик числа запуской второй программы
    uint16_t all = 0;       ///< Счетчик числа запуска обеих программ
};

int main()
{
    try
    {
        shared_memory_object shm_obj (open_or_create,
                                      "lab10_shared_memory",
                                      read_write
                                      );

        // Объявляем указатель на структуру с счётчиками.
        Counters* c = nullptr;

        // Область разделяемой памяти.
        mapped_region region;

        // Семафор.
        named_semaphore sem (open_or_create,
                             "lab10_2_sem", 1
                             );

        // Занимаем семафор для защиты критической секции.
        sem.wait();

        offset_t size = 0;
        shm_obj.get_size(size);

        // Если впервые создаем разделяемую память
        if (size == 0)
        {
            // Инициализируем структуру.
            Counters newCounters;
            c = &newCounters;

            c->first = 1;
            c->second = 0;
            c->all = 1;

            // Выделяем под нее место.
            shm_obj.truncate(sizeof(Counters));

            // Записываем ее в разделяемую память.
            region = {shm_obj, read_write};
            copy_n(c, sizeof(Counters), reinterpret_cast<Counters*>(region.get_address()));
        }
        // Если разделяемая память уже создана
        else
        {
            // Достаём структуру из разделяемой памяти.
            region = {shm_obj, read_write};
            c = reinterpret_cast<Counters*>(region.get_address());

            c->first++;
            c->all++;
        }

        // Печатаем новые значения счетчиков.
        cout << "Program 1 was spawn " << c->first << " times, program 2: "
             << c->second << " times, total: " << c->all << " times." << endl;

        // Освобождаем семафор.
        sem.post();

    }
    catch (interprocess_exception &ex)
    {
        cout << ex.what() << endl;
        return 1;
    }

    return 0;
}
