/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:21:53 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/18 15:13:56 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

client::client()
{
    // std::cout << "Client default constructor" << std::endl;
}

client::~client()
{
    // std::cout << "Client destructor" << std::endl;
}

void    client::initClient()
{
    _sa.sin_family = AF_INET; 
    _sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    _sa.sin_port = htons(PORT);
}

void    client::initSocket()
{
    _socketFd = socket(_sa.sin_family, SOCK_STREAM, 0);
    if (_socketFd == -1)
        socketFdError();
}

void    client::connectServer()
{
    _status = connect(_socketFd, (struct sockaddr *)&_sa, sizeof(_sa));
    if (_status != 0)
        connectError();
    std::cout << "PORT : " << PORT << ", connecting to localhost..." << std::endl;
}

void   client::handleMessage(void)
{
    if (_msg == "/QUIT")
        throw exitSocket();
}

void    client::sendMessage()
{
    std::getline(std::cin, _msg);
    handleMessage();
    _msgLen = _msg.size();
    _bytesSent = send(_socketFd, _msg.c_str(), _msgLen, 0);
    if (_bytesSent == -1)
        sendError();
    else if (_bytesSent != _msgLen)
        std::cout << "Partial message send, only : " << _bytesSent << "bytes sent" << std::endl;
}

void    client::receivedMessage()
{
    char buffer[BUFSIZ];
    _bytesRead = 0;
    while (_bytesRead >= 0) 
    {
        _bytesRead = recv(_socketFd, buffer, BUFSIZ, 0);
        if (_bytesRead == -1)
            recvError();
        else if (_bytesRead == 0)
        {
            std::cout << "Connection closed by peer." << std::endl;
            exit(0);
        }
        else 
        {
            buffer[_bytesRead] = '\0';
            std::cout << buffer << std::endl;
            break ;
        }
    }
}

void    client::run()
{
    try
    {
        initClient();
        initSocket();
        connectServer();
        
        struct pollfd fds[2];
        fds[0].fd = _socketFd;
        fds[0].events = POLLIN;
        fds[1].fd = STDIN_FILENO;
        fds[1].events = POLLIN;
        
        while (true)
        {
            int retSelect = poll(fds, 2, -1);

            if (retSelect < 0)
            {
                std::cout << "Error on poll" << std::endl;
                break ;
            }
            if (fds[0].revents & POLLIN)
                receivedMessage();
            if (fds[1].revents & POLLIN)
                sendMessage();
        }
    }
    catch(const std::exception& e)
    {
       std::cerr << e.what() << '\n';
    }
    close(_socketFd);
}

void    client::setMessage(std::string msg)
{
    _msg = msg;
}

const char* client::exitSocket::what() const throw()
{
    return ("We are closing socket");
}
const char* client::socketFdError::what() const throw()
{
    return ("Error on socket fd");
}

const char* client::sendError::what() const throw()
{
    return ("Error on send");
}

const char* client::recvError::what() const throw()
{
    return ("Error on recv");
}

const char* client::connectError::what() const throw()
{
    return ("error on connnect");
}