import React from 'react';
import { Container } from "react-bootstrap";
import { Socket } from "socket.io-client";
import { ChatUsersContainer } from "../components/ChatUsersContainer";

interface PrivateChatListPageParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
}
export const PrivateChatListPage: React.FC<PrivateChatListPageParams> = ({ socket }) =>
{
    return (
        <Container id="public-chat-page" className="d-flex flex-column h-100">
            <h3 className="text-center p-4">Список пользователей</h3>
            <ChatUsersContainer socket={socket} />
        </Container>
    );
};