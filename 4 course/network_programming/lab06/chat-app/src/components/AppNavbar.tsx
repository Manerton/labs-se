import React from 'react';
import { Container, Nav, Navbar, Row } from "react-bootstrap";
import { NavLink } from 'react-router-dom';

export const AppNavbar: React.FC = () =>
(
    <Row id="navbar-holder">
        <Navbar bg="dark" variant="dark">
            <div className = "d-flex">
                <Navbar.Brand>
                    <img
                        alt=""
                        src="/app.ico"
                        width="30"
                        height="30"
                        className="d-inline-block align-top"
                    />
                </Navbar.Brand>
                <Nav className="me-auto">
                    <NavLink to="/" className="nav-link">Общий</NavLink>
                    <NavLink to="/group-chat" className="nav-link">Групповой</NavLink>
                    <NavLink to="/private-chat" className="nav-link">Личный</NavLink>
                </Nav>
            </div>
        </Navbar>
    </Row>
);