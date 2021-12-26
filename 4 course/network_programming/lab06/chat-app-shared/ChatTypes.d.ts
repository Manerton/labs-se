export interface Message
{
    /** Веб-сокет Id пользователя. */
    socketId: string;
    /** Имя пользователя. */
    username: string;
    /** Текст сообщения. */
    content: string;
    /** Через Date.now(). */
    time: number
}