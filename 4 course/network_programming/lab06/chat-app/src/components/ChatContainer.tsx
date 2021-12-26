import React, { useEffect, useRef, useState } from "react";
import { Button, Container, Form, FormControl, InputGroup } from "react-bootstrap";
import { Socket } from "socket.io-client";
import { Message, MessageWithUser } from "../../../chat-app-shared/ChatTypes";
import { subscribeToNewMessages } from "../hooks/ChatContainerHooks";
import { messageEv, newUsernameEv } from "../SocketEvents";
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
    const [messageList, setMessageList] = useState<MessageWithUser[]>([]);
    const [username, setUsername] = useState<string>("");
    const messageRef = useRef<HTMLTextAreaElement>(null);
    const usernameRef = useRef<HTMLInputElement>(null);
    const fileRef = useRef<HTMLInputElement>(null);

    /** Выполнить после создание компонента или при изменении объекта socket. */
    useEffect(() =>
    {
        // Подписываемся на получение сообщений.
        return subscribeToNewMessages({ channelId, socket, setMessageList });
    }, [socket]);

    useEffect(() =>
    {
        console.log("Восстанавливаем никнейм из локального хранилища.");
        const name = localStorage.getItem("username");
        if (name)
        {
            setUsername(name);
            // Отправляем на сервер имя.
            socket.emit(newUsernameEv, name);
        }
        else
        {
            setUsername("guest");
        }
    }, [setUsername]);

    /** Изменить имя пользователя. */
    const changeUsername = () =>
    {
        const newName = usernameRef.current!.value;

        // Обновляем состояние имени.
        setUsername(newName);

        // Отправляем на сервер имя.
        socket.emit(newUsernameEv, newName);

        // Сохраняем его в локальном хранилище.
        localStorage.setItem("username", newName);

        console.log(`Изменен ник на ${newName}`);
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
                content: msg,
                time: Date.now()
            };

            const myMsgWithUser: MessageWithUser = {
                user: { id: socket.id, name: username },
                content: msg,
                time: Date.now()
            };

            // Добавляем его в список сообщений на клиенте.
            setMessageList(prev => [...prev, myMsgWithUser]);

            // Затираем поле для ввода сообщения.
            messageRef.current.value = "";

            // Отправляем сообщение на сервер.
            socket.emit(messageEv, myMsg);
        }
    };

    const fileUpload = async (file: File) =>
    {
        return new Promise<string>((resolve, reject) =>
        {
            const utf8_to_b64 = (str: string) =>
            {
                return window.btoa(unescape(encodeURIComponent(str)));
            };

            console.log("Отправляем файл:", file.name, file.size);

            const xhr = new XMLHttpRequest();
            xhr.open("POST", `${location.origin}/files`, true);

            xhr.setRequestHeader("Upload-Filename", utf8_to_b64(file.name));

            xhr.addEventListener("load", () =>
            {
                console.log(xhr.status);
                resolve(xhr.responseText);
            });

            xhr.send(file);
        });
    };

    /** Отправить файл. */
    const sendFile = async () =>
    {
        const file = fileRef.current!.files?.item(0);
        console.log(file);

        // Если нечего загружать.
        if (!file) return;

        // загружаем файл
        const fileId = await fileUpload(file);

        console.log(location.host);

        // локально добавляем ссылку на файл в чат
        messageRef.current!.value = `${location.origin}/files/${fileId}`;

        sendMessage();
    };

    const handleEnterKey = (event: React.KeyboardEvent) =>
    {
        if (event.key == "Enter")
        {
            event.preventDefault();
            sendMessage();
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
                    onKeyDown={handleEnterKey}
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
            <InputGroup>
                <Form.Control id="input-file" type="file" ref={fileRef} />
                <Button variant="outline-warning" onClick={sendFile}>
                    <img
                        alt=""
                        src="/upload.svg"
                        width="25"
                        height="25"
                        className="d-inline-block align-top"
                    />
                </Button>
            </InputGroup>
        </Container>
    );
};