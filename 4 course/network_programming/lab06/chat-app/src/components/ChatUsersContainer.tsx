import React, { useEffect, useState } from "react";
import { Container, ListGroup } from "react-bootstrap";
import { useNavigate } from "react-router-dom";
import { Socket } from "socket.io-client";
import { User } from "../../../chat-app-shared/ChatTypes";
import { subscribeToNewUsers } from "../hooks/ChatUsersContainerHooks";
import { privateChannelEv } from "../SocketEvents";

interface ChatUsersContainerParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
}
export const ChatUsersContainer: React.FC<ChatUsersContainerParams> = ({ socket }) =>
{
    const [userList, setUserList] = useState<User[]>([]);
    const navigate = useNavigate();

    /** Выполнить после создание компонента или при изменении объекта socket. */
    useEffect(() =>
    {
        // Подписываемся на получение юзеров.
        return subscribeToNewUsers({ socket, setUserList });
    }, [socket]);

    const getPrivateChannel = (
        event: React.MouseEvent,
        userId: string
    ) =>
    {
        console.log("Получаем Id личного канала с юзером:", userId);

        socket.emit(privateChannelEv, userId);

        socket.once(privateChannelEv, (channelId: string) =>
        {
            navigate(`/channels/${channelId}`);
        });
    };

    return (
        <Container id="chat-users-container" className="d-flex flex-column align-items-center h-80">
            <ListGroup as="ol" className="w-100 overflow-auto h-80">
                {
                    userList.map((user: User) =>
                    {
                        if (user.id != socket.id)
                        {
                            return (
                                <ListGroup.Item
                                    as="a"
                                    className="d-flex justify-content-between align-items-start"
                                    key={user.id}
                                    action
                                    variant="light"
                                    onClick={event => getPrivateChannel(event, user.id)}
                                >
                                    <div className="ms-2 me-auto fw-bold">
                                        {user.name} ({user.id})
                                    </div>
                                </ListGroup.Item>
                            );
                        }
                    })
                }
            </ListGroup>
        </Container>
    );
};