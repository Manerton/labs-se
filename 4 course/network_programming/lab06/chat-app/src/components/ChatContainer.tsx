import React, { FormEvent, useEffect, useRef, useState } from "react";
import { Button, Container, Form, FormControl, InputGroup, Row } from "react-bootstrap";
import { Socket } from "socket.io-client";
import { subscribeToNewMessages } from "../hooks/ChatContainerHooks";
import { ChatMessages } from "./ChatMessages";

interface ChatContainerParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
    /** Канал сообщений. */
    channelId: string;
}

export interface Message
{
    socketId: string;
    username: string;
    content: string;
    time: Date;
}
export const ChatContainer: React.FC<ChatContainerParams> = ({ channelId, socket }) =>
{
    const [messageList, setMessageList] = useState<Message[]>([]);
    const [username, setUsername] = useState<string>("guest");
    const messageRef = useRef<HTMLTextAreaElement>(null);

    useEffect(() =>
    {
        console.log(username);
        return subscribeToNewMessages({ channelId, socket, setMessageList });
    }, [socket]);

    const handleUsernameChange = (event: React.ChangeEvent<HTMLInputElement>) =>
    {
        setUsername(event.target.value);
    };

    const changeUsername = () =>
    {
        console.log(`Изменен ник на ${username}`);
    };

    const sendMessage = () =>
    {
        const msg = messageRef.current?.value;
        if (msg)
        {
            console.log(`Отправлено сообщение: ${msg}`);
            const testMsg: Message = {
                socketId: socket.id,
                username,
                content: msg,
                time: new Date()
            };

            setMessageList(prev => [...prev, testMsg]);
        }
    };

    return (
        <Container id="chat-container" className="d-flex flex-column align-items-center h-90">
            <InputGroup className="m-3">
                <InputGroup.Text>Имя</InputGroup.Text>
                <FormControl
                    placeholder="Имя пользователя"
                    onChange={handleUsernameChange}
                    value={username}
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