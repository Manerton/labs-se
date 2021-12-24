import React, { useEffect, useState } from "react";
import "bootstrap/dist/css/bootstrap.min.css";
import io, { Socket } from "socket.io-client";
import { BrowserRouter } from "react-router-dom";

import { Navbar } from "./components/Navbar";
import { MyPageRouter } from "./components/MyPageRouter";

const App: React.FC = () =>
{
    /// Веб-сокет соединение.
    const [socket, setSocket] = useState<Socket>();
    /// Активно ли соединение?
    const [socketConnected, setSocketConnected] = useState<boolean>(false);

    useEffect(() =>
    {
        console.log("AppUseEffect");
        const newSocket = io("https://192.168.1.4/", { 'transports': ['websocket'] });
        setSocket(newSocket);

        newSocket.on('connect', () =>
        {
            console.log(newSocket.id);
            setSocketConnected(true);
        });

        newSocket.on('disconnect', () =>
        {
            setSocketConnected(false);
        });

        return () =>
        {
            newSocket.close();
        };
    }, [setSocket]);

    return (
        <BrowserRouter>
            <Navbar />
            {(socketConnected) ? (
                <MyPageRouter socket={socket!} />
            ) : (
                <div>Нет соединения...</div>
            )}
        </BrowserRouter>
    );
};

export default App;
