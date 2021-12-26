// Стили
import "bootstrap/dist/css/bootstrap.min.css";

// Готовые компоненты
import React, { useEffect, useState } from "react";
import io, { Socket } from "socket.io-client";
import { BrowserRouter } from "react-router-dom";
import { Container, Row, Spinner } from "react-bootstrap";

// Мои компоненты
import { AppNavbar } from "./components/AppNavbar";
import { MyPageRouter } from "./components/MyPageRouter";

const WEBSOCKET_SERVER_PATH = "/";

const App: React.FC = () =>
{

    /** Веб-сокет соединение. */
    const [socket, setSocket] = useState<Socket>();

    /** Удалось ли веб-сокет соединение? */
    const [socketConnected, setSocketConnected] = useState<boolean>(false);

    /**
     * Создать веб-сокет подключение к серверу.
     */
    const createSocket = () =>
    {
        console.log("Создаю веб-сокет соединение...");
        const newSocket = io(WEBSOCKET_SERVER_PATH, { 'transports': ['websocket'] });

        setSocket(newSocket);

        newSocket.on('connect', () =>
        {
            console.log(`Соединение установлено: ${newSocket.id}`);
            setSocketConnected(true);
        });

        newSocket.on('disconnect', () =>
        {
            setSocketConnected(false);
        });

        return () =>
        {
            console.log("Закрываю веб-сокет соединение...");
            newSocket.close();
        };
    };

    useEffect(() =>
    {
        return createSocket();
    }, [setSocket]);

    return (
        <BrowserRouter>
            <Container id="app" fluid className="d-flex flex-column vh-100">
                <AppNavbar />
                {(socketConnected) ? (
                    <MyPageRouter socket={socket!} />
                ) : (
                    <Row className="flex-grow-1 align-items-center justify-content-center">
                        <Spinner animation="border" />
                    </Row>
                )}
            </Container>
        </BrowserRouter>
    );
};

export default App;
