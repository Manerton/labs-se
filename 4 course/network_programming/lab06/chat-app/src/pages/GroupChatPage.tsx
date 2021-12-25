import React from 'react';
import { Container } from "react-bootstrap";
import { useParams } from "react-router-dom";
import { Socket } from "socket.io-client";
import { ChatContainer } from "../components/ChatContainer";
interface GroupChatPageParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
}
export const GroupChatPage: React.FC<GroupChatPageParams> = ({socket}) =>
{
    const { id } = useParams();

    return (
        <Container id={`group-chat-page-${id!}`} className="d-flex flex-column h-100">
            <h1>Групповой чат: {id}</h1>
            <ChatContainer channelId={id!} socket={socket} ></ChatContainer>
        </Container>
    );
};