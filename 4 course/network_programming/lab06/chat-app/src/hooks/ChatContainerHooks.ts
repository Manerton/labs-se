import { Dispatch, SetStateAction } from "react";
import { Socket } from "socket.io-client";
import { Message } from "../components/ChatContainer";

type SubscribeToNewMessagesParams = {
    channelId: string,
    socket: Socket,
    setMessageList: Dispatch<SetStateAction<Message[]>>;
};

export const subscribeToNewMessages = (params: SubscribeToNewMessagesParams) =>
{
    const { channelId, socket, setMessageList } = params;
    console.log(`Подписываемся на получение сообщений канала ${channelId}.`);
    socket.emit("ready-receive-messages", "public");

    const testMsg: Message = {
        socketId: "test",
        username: "anon",
        content: "Hello world",
        time: new Date()
    };

    setMessageList(prev => [...prev, testMsg]);

    // Деструктор.
    return () =>
    {
        console.log(`Отписываемся от получения сообщений канала ${channelId}.`);
    };
};
