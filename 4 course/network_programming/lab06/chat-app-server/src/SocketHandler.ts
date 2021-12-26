import https = require('https');
import { nanoid } from "nanoid";
import { ChatChannel, Message, MessageWithUser, P2PChatChannel, User } from "../../chat-app-shared//ChatTypes";
import { channelEv, doneReceiveChannelsEv, doneReceiveMessagesEv, doneReceiveUsersEv, messageEv, newUsernameEv, privateChannelEv, readyReceiveChannelsEv, readyReceiveMessagesEv, readyReceiveUsersEv, userJoinEv, userLeaveEv } from "./SocketEvents";
import SocketIO = require('socket.io');

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

    /** Список пользователей на сервере. */
    private userList = new Map<string, User>();

    /** Список каналов на сервере. */
    private channelList: ChatChannel[] = [];

    /** Личные каналы пользователей (ключ - channelId) */
    private privateChannelList = new Map<string, P2PChatChannel>();

    /** История сообщений канала channelId. */
    private messageList = new Map<string, MessageWithUser[]>();

    constructor(
        _server: https.Server
    )
    {
        this.io = this.createSocketServer(_server);

        // Создаем для общего чата историю сообщений.
        this.messageList.set("public", []);

        // [Главная страница]
        this.io.of('/').on('connection', (socket: Socket) =>
        {
            console.log("Новое соединение:", socket.id);

            // Запоминаем юзера в список.
            this.userList.set(socket.id, ({ id: socket.id, name: "guest" }));

            // Сообщаем всем, кто за этим следит (группа users), что зашел новый юзер.
            this.emitTo("users", userJoinEv, this.userList.get(socket.id));

            // Пользователь поменял никнейм.
            socket.on(newUsernameEv, (name: string) =>
            {
                this.userList.get(socket.id)!.name = name;
                const user = this.userList.get(socket.id)!;
                this.emitTo("users", newUsernameEv, user);
            });

            // Готов получать и отправлять сообщения из канала channelId.
            socket.on(readyReceiveMessagesEv, async (channelId: string) =>
            {
                // если комнаты нет (точнее истории сообщений комнаты)
                if (!this.messageList.get(channelId))
                {
                    return;
                }

                const privateChannel = this.privateChannelList.get(channelId);

                // если это приватный личный чат
                // и этот юзер не имеет права в нем находится
                if (privateChannel
                    && (privateChannel.firstUserId != socket.id
                        && privateChannel.secondUserId != socket.id)
                )
                {
                    return;
                }

                console.log(`${readyReceiveMessagesEv as string} from`, socket.id);
                // Входим в группу сокетов по названию channelId
                await socket.join(channelId);

                // Отправляем историю сообщений юзеру.
                for (const msg of this.messageList.get(channelId)!)
                {
                    socket.emit(messageEv, msg);
                }

                // Клиент прислал сообщение.
                socket.on(messageEv, (msg: Message) =>
                {
                    const msgWithUser: MessageWithUser =
                    {
                        user: this.userList.get(socket.id)!,
                        content: msg.content,
                        time: msg.time
                    };

                    // Обновляем список.
                    this.messageList.get(channelId)!.push(msgWithUser);
                    socket.to(channelId).emit(messageEv, msgWithUser);
                });
            });

            // Больше не собирается получать и отправлять сообщения из канала channelId
            socket.on(doneReceiveMessagesEv, async (channelId: string) =>
            {
                console.log(`${doneReceiveMessagesEv as string} from`, socket.id);
                // Покидаем группу сокетов по названию channelId.
                await socket.leave(channelId);
                // Прекращаем получать сообщения от юзера.
                socket.removeAllListeners(messageEv);
            });

            // Готов получать и создавать каналы.
            socket.on(readyReceiveChannelsEv, () =>
            {
                console.log(`${readyReceiveChannelsEv as string} from`, socket.id);

                // Отправляем список каналов юзеру.
                for (const channel of this.channelList)
                {
                    socket.emit(channelEv, channel);
                }

                // Клиент создал новый канал.
                socket.on(channelEv, (title: string) =>
                {
                    const channelId = nanoid();
                    const newChannel: ChatChannel = { channelId, title };

                    // Обновляем список.
                    this.channelList.push(newChannel);

                    // Создадим историю чата для этого канала.
                    this.messageList.set(channelId, []);

                    // Сообщаем об этом всем (включая создававшего,
                    // ведь он не знает Id созданного канала).
                    this.emitToAll(channelEv, newChannel);
                });
            });

            // Больше не собирается получать и создавать каналы.
            socket.on(doneReceiveChannelsEv, () =>
            {
                console.log(`${doneReceiveChannelsEv as string} from`, socket.id);
                // Прекращаем получать каналы от юзера.
                socket.removeAllListeners(channelEv);
            });

            // Готов получать инфу о новых и вышедших пользователях.
            socket.on(readyReceiveUsersEv, async () =>
            {
                console.log(`${readyReceiveUsersEv as string} from`, socket.id);

                // Отправляем текущий список пользователей.
                for (const user of this.userList.values())
                {
                    socket.emit(userJoinEv, user);
                }

                // Заходим в комнату, куда отправляется инфа
                // о подключившихся и отключившихся.
                await socket.join("users");
            });

            // Больше не собирается получать инфу о пользователях.
            socket.on(doneReceiveUsersEv, async () =>
            {
                console.log(`${doneReceiveUsersEv as string} from`, socket.id);
                await socket.leave("users");
            });

            socket.on(privateChannelEv, (userId: string) =>
            {
                const findPrivateChannel = () =>
                {
                    for (const channel of this.privateChannelList.values())
                    {
                        if (this.checkUsersForPrivateChannel(channel, userId, socket.id))
                        {
                            return channel;
                        }
                    }
                };

                // Ищем его, возможно он уже создан
                let privateChannel = findPrivateChannel();

                // Если его нет, то создаём
                if (!privateChannel)
                {
                    const channelId = nanoid();
                    privateChannel = {
                        channelId,
                        firstUserId: socket.id,
                        secondUserId: userId,
                        title: "private"
                    };
                    this.privateChannelList.set(channelId, privateChannel);
                    // Создадим историю чата для этого канала.
                    this.messageList.set(channelId, []);
                }

                // Отправляем id канала
                socket.emit(privateChannelEv, privateChannel.channelId);
            });

            socket.on("disconnect", () =>
            {
                // Пользователь вышел, сообщаем всем, кто за этим следит.
                console.log(`User ${socket.id} has disconnected.`);
                this.emitTo("users", userLeaveEv, socket.id);

                // Удаляем юзера из списка.
                this.userList.delete(socket.id);
            });
        });
    }

    private checkUsersForPrivateChannel(
        channel: P2PChatChannel,
        firstUserId: string,
        secondUserId: string
    ): boolean
    {
        return ((channel.firstUserId == firstUserId || channel.secondUserId == firstUserId)
            && (channel.firstUserId == secondUserId || channel.secondUserId == secondUserId));
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