#ifndef POSIX_MESSAGE_QUEUE_H
#define POSIX_MESSAGE_QUEUE_H

#include <cstdlib>
#include <cstdint>

#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */
#include <mqueue.h>
#include <cerrno>

#include <exception>
#include <stdexcept>
#include <string>

/// Только создать очередь.
struct create_only_t {};

/// Открыть существующую или создать в случае отсутствия очереди.
struct open_or_create_t {};

/// Только открыть очередь.
struct open_only_t {};

/// Режим открытия очереди.
enum open_mode_t
{
    read_only = O_RDONLY,
    write_only = O_WRONLY,
    read_and_write = O_RDWR
};

/// Класс-обертка над POSIX Message Queue
class posix_message_queue
{
    /// Дескриптор очереди.
    mqd_t desc = 0;

    /// Аттрибуты очереди.
    mq_attr attr {};

    void throwError()
    {
        throw std::runtime_error("errno: " + std::to_string(errno));
    }

    void getAttr()
    {
        auto res = mq_getattr(desc, &attr);

        if (res == -1)
        {
            throwError();
        }
    }

public:
    using signed_size_type = __syscall_slong_t;
    using unsigned_size_type = size_t;

    /**
     * @brief Создает очередь сообщений с именем "name".
     * Выбрасывает исключение в случае если очередь уже существует.
     * @param create_only Тэг - создать очередь.
     * @param open_mode Режим открытия очереди (чтение/запись).
     * @param name Название очереди.
     * @param max_num_msg Максимальное количество сообщений в очереди.
     * @param max_msg_size Максимальный размер одного сообщения.
     */
    posix_message_queue(create_only_t /* create_only */,
                        open_mode_t open_mode,
                        const char *name,
                        signed_size_type max_num_msg,
                        signed_size_type max_msg_size)
    {
        attr.mq_flags = 0;
        attr.mq_curmsgs = 0;
        attr.mq_maxmsg = max_num_msg;
        attr.mq_msgsize = max_msg_size;

        std::string correct_name = "/" + std::string(name);

        this->desc = mq_open(correct_name.c_str(), open_mode | O_CREAT | O_EXCL, S_IRWXU, &attr);

        if (this->desc == -1)
        {
            throwError();
        }
    }

    /**
     * @brief Открывает очередь сообщений с именем "name" или создает, если очередь не существует.
     * @param open_or_create Тэг - создать или открыть очередь.
     * @param open_mode Режим открытия очереди (чтение/запись).
     * @param name Название очереди.
     * @param max_num_msg Максимальное количество сообщений в очереди.
     * @param max_msg_size Максимальный размер одного сообщения.
     */
    posix_message_queue(open_or_create_t /* open_or_create */,
                        open_mode_t open_mode,
                        const char *name,
                        signed_size_type max_num_msg,
                        signed_size_type max_msg_size)
    {
        attr.mq_flags = 0;
        attr.mq_curmsgs = 0;
        attr.mq_maxmsg = max_num_msg;
        attr.mq_msgsize = max_msg_size;

        std::string correct_name = "/" + std::string(name);

        this->desc = mq_open(correct_name.c_str(), open_mode | O_CREAT, S_IRWXU, &attr);

        if (this->desc == -1)
        {
            throwError();
        }
    }

    /**
     * @brief Открывает очередь сообщений с именем "name".
     * Выбрасывает исключение в случае если очереди не существует.
     * @param open_only Тэг - открыть очередь.
     * @param open_mode Режим открытия очереди (чтение/запись).
     * @param name Название очереди.
     */
    posix_message_queue(open_only_t /* open_only */,
                        open_mode_t open_mode,
                        const char *name)
    {
        std::string correct_name = "/" + std::string(name);

        this->desc = mq_open(correct_name.c_str(), open_mode);

        if (this->desc == -1)
        {
            throwError();
        }

        this->getAttr();
    }

    ~posix_message_queue()
    {
        auto res = mq_close(this->desc);

        if (res == -1)
        {
            throwError();
        }
    }

    /**
     * @brief Отправляет сообщение из буфера "buffer" с
     * размером "buffer_size" с приоритетом "priority".
     */
    void send (const char *buffer,
               unsigned_size_type buffer_size,
               uint32_t priority)
    {
        auto res = mq_send(desc, buffer, buffer_size, priority);

        if (res == -1)
        {
            throwError();
        }

        this->getAttr();
    }

    /**
     * @brief Принимает сообщение в буфер "buffer" с размером "buffer_size".
     * Размер принятого сообщение и приоритет помещается в "recvd_size" и "priority" соответственно.
     */
    void receive (char *buffer,
                  unsigned_size_type buffer_size,
                  signed_size_type &recvd_size,
                  uint32_t &priority)
    {
        auto res = mq_receive(desc, buffer, buffer_size, &priority);

        if (res == -1)
        {
            throwError();
        }

        recvd_size = res;
        this->getAttr();
    }

    /// Возвращает максимальное количество разрешенных сообщений в этой очереди.
    signed_size_type get_max_msg() const
    {
        return attr.mq_maxmsg;
    }

    /// Возвращает максимальный размер сообщения для этой очереди.
    signed_size_type get_max_msg_size() const
    {
        return attr.mq_msgsize;
    }

    /// Возвращает текущее количество сообщений в очереди.
    signed_size_type get_num_msg() const
    {
        return attr.mq_curmsgs;
    }

    /**
     * @brief Удаляет очередь из системы.
     * @param name Название очереди.
     * @return False в случае ошибки.
     */
    static bool remove(const char *name)
    {
        std::string correct_name = "/" + std::string(name);

        auto res = mq_unlink(correct_name.c_str());
        return (res != -1);
    }
};

#endif // POSIX_MESSAGE_QUEUE_H
