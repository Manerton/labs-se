import React, { useEffect, useRef, useState } from "react";
import { Button, Container, FormControl, InputGroup } from "react-bootstrap";
import { Socket } from "socket.io-client";
import { ChatChannel } from "../../../chat-app-shared/ChatTypes";
import { createChannelOnServer, subscribeToNewChannels } from "../hooks/ChatChannelContainerHooks";
import { ChatChannels } from "./ChatChannels";

interface ChatChannelListParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
}

export const ChatChannelContainer: React.FC<ChatChannelListParams> = ({ socket }) =>
{
    const [channelList, setChannelList] = useState<ChatChannel[]>([]);
    const newChannelTitleRef = useRef<HTMLInputElement>(null);

    /** Выполнить после создание компонента или при изменении объекта socket. */
    useEffect(() =>
    {
        // Подписываемся на получение каналов.
        return subscribeToNewChannels({ socket, setChannelList });
    }, [socket]);

    /** Создать новый канал. */
    const createChannel = () =>
    {
        const newChannelTitle = newChannelTitleRef.current!.value;
        createChannelOnServer({ socket, title: newChannelTitle });
        console.log(`Создан новый канал с названием ${newChannelTitle}`);
    };

    return (
        <Container id="chat-channel-container" className="d-flex flex-column align-items-center h-80">
            <InputGroup className="m-3">
                <InputGroup.Text>Название</InputGroup.Text>
                <FormControl
                    placeholder="Название канала"
                    ref={newChannelTitleRef}
                />
                <Button variant="secondary" onClick={createChannel}>
                    Создать
                </Button>
            </InputGroup>
            <ChatChannels channelList={channelList} />
        </Container>
    );
};