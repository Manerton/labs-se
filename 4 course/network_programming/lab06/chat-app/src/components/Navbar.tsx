import React from 'react';
import { NavLink } from 'react-router-dom';

export const Navbar: React.FC = () =>
(
    <nav className="navbar navbar-expand-lg navbar-dark bg-dark">
        <div className="container-fluid">
            <a className="navbar-brand">Чат</a>
            <div className="collapse navbar-collapse" id="navbarNav">
                <ul className="navbar-nav">
                    <li className="nav-item">
                        <NavLink to="/" className="nav-link">Общий</NavLink>
                    </li>
                    <li className="nav-item">
                        <NavLink to="/group-chat" className="nav-link">Групповой</NavLink>
                    </li>
                    <li className="nav-item">
                        <NavLink to="/private-chat" className="nav-link">Личный</NavLink>
                    </li>
                </ul>
            </div>
        </div>
    </nav>
);