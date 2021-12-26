import https = require('https');
import SocketIO = require('socket.io');
import { Message } from "../../chat-app-shared/ChatTypes";

export type SocketId = string;
type Socket = SocketIO.Socket;

// класс - обработчик сокетов
export class SocketHandler
{
    private io: SocketIO.Server;
    private createSocketServer(server: https.Server): SocketIO.Server
    {
        return new SocketIO.Server(server, {
            transports: ['websocket'],
            pingInterval: 5000,
            pingTimeout: 15000,
            serveClient: false
        });
    }

    constructor(
        _server: https.Server
    )
    {
        this.io = this.createSocketServer(_server);

        // [Главная страница]
        this.io.of('/').on('connection', (socket: Socket) =>
        {
            console.log("Новое соединение:", socket.id);

            // Готов получать сообщения из канала channelId
            socket.on("ready-receive-messages", (channelId: string) =>
            {
                console.log("ready-receive-messages from", socket.id);
                // Входим в группу сокетов по названию channelId
                void socket.join(channelId);

                socket.on("message", (msg: Message) =>
                {
                    socket.to(channelId).emit("message", msg);
                });
            });
        });
    }

    public emitTo(name: string, ev: string, ...args: any[]): boolean
    {
        return this.io.of('/').to(name).emit(ev, ...args);
    }

    public emitToAll(ev: string, ...args: any[]): boolean
    {
        return this.io.of('/').emit(ev, ...args);
    }

    public getSocketsCount(): number
    {
        return this.io.of('/').sockets.size;
    }
}