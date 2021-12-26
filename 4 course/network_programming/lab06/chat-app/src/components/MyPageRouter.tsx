import React from "react";

import { Routes, Route } from "react-router-dom";

import { PublicChatPage } from "../pages/PublicChatPage";
import { GroupChatListPage } from "../pages/GroupChatListPage";
import { GroupChatPage } from "../pages/GroupChatPage";
import { PrivateChatListPage } from "../pages/PrivateChatListPage";
import { Socket } from "socket.io-client";
import { Row } from "react-bootstrap";

interface MyPageRouterParams
{
    /** Веб-сокет соединение с сервером. */
    socket: Socket;
}

export const MyPageRouter: React.FC<MyPageRouterParams> = ({ socket }) =>
{
    return (
        <Row id="main" className="overflow-hidden h-100">
            <Routes>
                <Route element={<PublicChatPage socket={socket} />} path="/" />
                <Route path="/group-chat">
                    <Route element={<GroupChatPage socket={socket} />} path=":id" />
                    <Route index element={<GroupChatListPage />} />
                </Route>
                <Route element={<PrivateChatListPage />} path="/private-chat" />
            </Routes>
        </Row>
    );
};