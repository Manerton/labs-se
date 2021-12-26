import React from 'react';
import { Container } from "react-bootstrap";
import { Socket } from "socket.io-client";
import { ChatChannelContainer } from "../components/ChatChannelContainer";

interface GroupChatListPageParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
}
export const GroupChatListPage: React.FC<GroupChatListPageParams> = ({ socket }) =>
{
    return (
        <Container id="public-chat-page" className="d-flex flex-column h-100">
            <h3 className="text-center p-4">Список групповых чатов</h3>
            <ChatChannelContainer socket={socket} />
        </Container>
    );
};