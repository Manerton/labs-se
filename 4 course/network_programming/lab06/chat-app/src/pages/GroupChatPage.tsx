import React from 'react';
import { useParams } from "react-router-dom";

export const GroupChatPage: React.FC = () =>
{
    const { id } = useParams();

    return (
        <>
            <h1>Групповой чат: {id}</h1>
        </>
    );
};