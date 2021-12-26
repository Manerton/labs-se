import { Dispatch, SetStateAction } from "react";
import { Socket } from "socket.io-client";
import { Message } from "../../../chat-app-shared/ChatTypes";

type SubscribeToNewMessagesParams = {
    /** Id канала сообщений. */
    channelId: string,
    /** Веб-сокет соединение. */
    socket: Socket,
    /** Функция для изменения списка сообщений в UI. */
    setMessageList: Dispatch<SetStateAction<Message[]>>;
};

/**
 * Подписаться на получение сообщений.
 */
export const subscribeToNewMessages = (params: SubscribeToNewMessagesParams) =>
{
    const { channelId, socket, setMessageList } = params;

    console.log(`Подписываемся на получение сообщений канала ${channelId}.`);

    socket.on("message", (msg: Message) =>
    {
        console.log("Получено сообщение: ", msg);
        setMessageList(prev => [...prev, msg]);
    });

    socket.emit("ready-receive-messages", channelId);

    // Деструктор.
    return () =>
    {
        console.log(`Отписываемся от получения сообщений канала ${channelId}.`);
        socket.off("message");
    };
};
