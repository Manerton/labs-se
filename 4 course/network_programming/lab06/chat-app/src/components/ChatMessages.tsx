import { Badge, ListGroup } from "react-bootstrap";
import { MessageWithUser } from "../../../chat-app-shared/ChatTypes";

interface ChatMessagesParams
{
    /** Веб-сокет соединение с сервером. */
    messageList: MessageWithUser[];
}
export const ChatMessages: React.FC<ChatMessagesParams> = ({ messageList }) =>
{
    return (
        <ListGroup as="ol" className="w-100 overflow-auto h-80">
            {
                messageList.map((message, index) =>
                {
                    return (
                        <ListGroup.Item
                            as="li"
                            className="d-flex justify-content-between align-items-start"
                            key={index}
                        >
                            <div className="ms-2 me-auto">
                                <div className="fw-bold">{message.user.name} ({message.user.id})</div>
                                {message.content}
                            </div>
                            <Badge bg="primary" pill>
                                {new Date(message.time).toLocaleTimeString()}
                            </Badge>
                        </ListGroup.Item>
                    );
                })
            }
        </ListGroup>
    );
};