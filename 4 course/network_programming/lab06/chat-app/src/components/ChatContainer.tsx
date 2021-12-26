import React, { useEffect, useRef, useState } from "react";
import { Button, Container, FormControl, InputGroup } from "react-bootstrap";
import { Socket } from "socket.io-client";
import { Message } from "../../../chat-app-shared/ChatTypes";
import { subscribeToNewMessages } from "../hooks/ChatContainerHooks";
import { ChatMessages } from "./ChatMessages";

interface ChatContainerParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
    /** Канал сообщений. */
    channelId: string;
}

export const ChatContainer: React.FC<ChatContainerParams> = ({ channelId, socket }) =>
{
    const [messageList, setMessageList] = useState<Message[]>([]);
    const [username, setUsername] = useState<string>("guest");
    const messageRef = useRef<HTMLTextAreaElement>(null);
    const usernameRef = useRef<HTMLInputElement>(null);

    /** Выполнить после создание компонента или при изменении объекта socket. */
    useEffect(() =>
    {
        // Подписываемся на получение сообщений.
        return subscribeToNewMessages({ channelId, socket, setMessageList });
    }, [socket]);

    /** Изменить имя пользователя. */
    const changeUsername = () =>
    {
        // Обновляем состояние имени.
        setUsername(usernameRef.current!.value);

        console.log(`Изменен ник на ${username}`);
    };

    /** Отправить сообщение. */
    const sendMessage = () =>
    {
        // Считываем сообщение.
        const msg = messageRef.current?.value;
        if (msg)
        {
            console.log(`Отправлено сообщение: ${msg}`);

            const myMsg: Message = {
                socketId: socket.id,
                username,
                content: msg,
                time: Date.now()
            };

            // Добавляем его в список сообщений на клиенте.
            setMessageList(prev => [...prev, myMsg]);

            // Затираем поле для ввода сообщения.
            messageRef.current.value = "";

            // Отправляем сообщение на сервер.
            socket.emit("message", myMsg);
        }
    };

    return (
        <Container id="chat-container" className="d-flex flex-column align-items-center h-80">
            <InputGroup className="m-3">
                <InputGroup.Text>Имя</InputGroup.Text>
                <FormControl
                    placeholder="Имя пользователя"
                    ref={usernameRef}
                    defaultValue={username}
                />
                <Button variant="secondary" onClick={changeUsername}>
                    Изменить
                </Button>
            </InputGroup>
            <ChatMessages messageList={messageList} />
            <InputGroup className="m-3">
                <FormControl
                    as="textarea"
                    ref={messageRef}
                />
                <Button variant="outline-warning" onClick={sendMessage}>
                    <img
                        alt=""
                        src="/send.svg"
                        width="25"
                        height="25"
                        className="d-inline-block align-top"
                    />
                </Button>
            </InputGroup>
        </Container>
    );
};