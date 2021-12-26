export interface Message
{
    /** Текст сообщения. */
    content: string;
    /** Через Date.now(). */
    time: number;
}
export interface User
{
    id: string;
    name: string;
}
export interface MessageWithUser extends Message
{
    user: User;
}
export interface ChatChannel
{
    channelId: string;
    title: string;
}

export interface P2PChatChannel extends ChatChannel
{
    firstUserId : string;
    secondUserId: string;
}
