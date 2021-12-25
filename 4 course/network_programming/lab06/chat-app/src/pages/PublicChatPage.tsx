import React from 'react';
import { Container } from "react-bootstrap";
import { Socket } from "socket.io-client";
import { ChatContainer } from "../components/ChatContainer";

interface PublicChatPageParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
}

export const PublicChatPage: React.FC<PublicChatPageParams> = ({ socket }) =>
{
    return (
        <Container id="public-chat-page" className="d-flex flex-column h-100">
            <h3 className="text-center p-4">Общий чат</h3>
            <ChatContainer channelId="public" socket={socket} />
        </Container>
    );
};