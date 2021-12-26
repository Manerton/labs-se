import { Dispatch, SetStateAction } from "react";
import { Socket } from "socket.io-client";
import { ChatChannel } from "../../../chat-app-shared/ChatTypes";
import { channelEv, readyReceiveChannelsEv, doneReceiveChannelsEv } from "../SocketEvents";

type SubscribeToNewChannelsParams = {
    /** Веб-сокет соединение. */
    socket: Socket,
    /** Функция для изменения списка сообщений в UI. */
    setChannelList: Dispatch<SetStateAction<ChatChannel[]>>;
};

/**
 * Подписаться на получение новых каналов.
 */
export const subscribeToNewChannels = (params: SubscribeToNewChannelsParams) =>
{
    const { socket, setChannelList } = params;

    console.log(`Подписываемся на получение каналов.`);

    socket.on(channelEv, (channel: ChatChannel) =>
    {
        console.log("Получен канал: ", channel);
        setChannelList(prev => [...prev, channel]);
    });

    socket.emit(readyReceiveChannelsEv);

    // Деструктор.
    return () =>
    {
        console.log("Отписываемся от получения каналов.");
        socket.off(channelEv);
        console.log("Сообщаем серверу, что больше не хотим получать каналы.");
        socket.emit(doneReceiveChannelsEv);
    };
};

type CreateChannelOnServerParams = {
    /** Веб-сокет соединение. */
    socket: Socket,
    /** Название нового канала. */
    title: string
};

/** Создать новый канал на сервере. */
export const createChannelOnServer = (params: CreateChannelOnServerParams) =>
{
    const { socket, title } = params;

    socket.emit(channelEv, title);
};

