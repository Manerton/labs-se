import React, { useEffect } from "react";

import { Routes, Route } from "react-router-dom";

import { PublicChatPage } from "../pages/PublicChatPage";
import { GroupChatListPage } from "../pages/GroupChatListPage";
import { GroupChatPage } from "../pages/GroupChatPage";
import { PrivateChatListPage } from "../pages/PrivateChatListPage";
import { Socket } from "socket.io-client";

type MyPageRouterParams = {
    socket: Socket;
};

export const MyPageRouter: React.FC<MyPageRouterParams> = ({socket}) =>
{
    useEffect(() =>
    {
        console.log("MyPageRouterEffect");
        socket.emit("test", "kyky");
    }, [socket]);

    return (
        <div className="container">
            <Routes>
                <Route element={<PublicChatPage />} path="/" />
                <Route path="/group-chat">
                    <Route element={<GroupChatPage />} path=":id" />
                    <Route index element={<GroupChatListPage />} />
                </Route>
                <Route element={<PrivateChatListPage />} path="/private-chat" />
            </Routes>
        </div>
    );
};