import { Dispatch, SetStateAction } from "react";
import { Socket } from "socket.io-client";
import { User } from "../../../chat-app-shared/ChatTypes";
import { doneReceiveUsersEv, newUsernameEv, readyReceiveUsersEv, userJoinEv, userLeaveEv } from "../SocketEvents";

type SubscribeToNewUsersParams = {
    /** Веб-сокет соединение. */
    socket: Socket,
    /** Функция для изменения списка сообщений в UI. */
    setUserList: Dispatch<SetStateAction<User[]>>;
};

/**
 * Подписаться на получение новых юзеров.
 */
export const subscribeToNewUsers = (params: SubscribeToNewUsersParams) =>
{
    const { socket, setUserList } = params;

    console.log(`Подписываемся на получение информации о пользователях.`);

    socket.on(userJoinEv, (user: User) =>
    {
        console.log("Новый пользователь: ", user);
        setUserList(prev => [...prev, user]);
    });

    socket.on(userLeaveEv, (userId: string) =>
    {
        console.log("Пользователь вышел: ", userId);
        setUserList(prev =>
            prev.filter(user => user.id !== userId));
    });

    socket.on(newUsernameEv, ({ id, name }: User) =>
    {
        console.log("Новый ник для пользователя:", id, name);
        setUserList(prev =>
            prev.map(user =>
            {
                if (user.id == id)
                {
                    user.name = name;
                }
                return user;
            })
        );
    });

    socket.emit(readyReceiveUsersEv);

    // Деструктор.
    return () =>
    {
        console.log("Отписываемся от получения событий пользователей.");
        socket.off(userJoinEv);
        socket.off(userLeaveEv);
        socket.off(newUsernameEv);
        console.log("Сообщаем серверу, что больше не хотим получать информацию о пользователях.");
        socket.emit(doneReceiveUsersEv);
    };
};