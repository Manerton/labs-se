import { Dispatch, SetStateAction } from "react";
import { Socket } from "socket.io-client";
import { MessageWithUser } from "../../../chat-app-shared/ChatTypes";
import { doneReceiveMessagesEv, messageEv, readyReceiveMessagesEv } from "../SocketEvents";

type SubscribeToNewMessagesParams = {
    /** Id канала сообщений. */
    channelId: string,
    /** Веб-сокет соединение. */
    socket: Socket,
    /** Функция для изменения списка сообщений в UI. */
    setMessageList: Dispatch<SetStateAction<MessageWithUser[]>>;
};

/**
 * Подписаться на получение сообщений.
 */
export const subscribeToNewMessages = (params: SubscribeToNewMessagesParams) =>
{
    const { channelId, socket, setMessageList } = params;

    console.log(`Подписываемся на получение сообщений канала ${channelId}.`);

    socket.on(messageEv, (msg: MessageWithUser) =>
    {
        console.log("Получено сообщение: ", msg);
        setMessageList(prev => [...prev, msg]);
    });

    socket.emit(readyReceiveMessagesEv, channelId);

    // Деструктор.
    return () =>
    {
        console.log(`Отписываемся от получения сообщений канала ${channelId}.`);
        socket.off(messageEv);
        console.log("Сообщаем серверу, что больше не хотим получать сообщения.");
        socket.emit(doneReceiveMessagesEv, channelId);
    };
};
