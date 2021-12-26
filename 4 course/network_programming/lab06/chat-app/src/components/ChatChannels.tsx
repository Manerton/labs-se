import { ListGroup } from "react-bootstrap";
import { Link } from 'react-router-dom';
import { ChatChannel } from "../../../chat-app-shared/ChatTypes";

interface ChatChannelsParams
{
    /** Веб-сокет соединение с сервером. */
    channelList: ChatChannel[];
}
export const ChatChannels: React.FC<ChatChannelsParams> = ({ channelList }) =>
{
    return (
        <ListGroup as="ol" className="w-100 overflow-auto h-80">
            {
                channelList.map((channel: ChatChannel) =>
                {
                    const channelPath = `/channels/${channel.channelId}`;
                    return (
                        <Link className="text-decoration-none m-1" to={channelPath}>
                            <ListGroup.Item
                                as="li"
                                className="d-flex justify-content-between align-items-start"
                                key={channel.channelId}
                                action
                                variant="light"
                            >
                                <div className="ms-2 me-auto fw-bold">
                                    {channel.title} ({channel.channelId})
                                </div>
                            </ListGroup.Item>
                        </Link>
                    );
                })
            }
        </ListGroup>
    );
};