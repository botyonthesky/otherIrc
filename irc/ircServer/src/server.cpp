/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:38:47 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/07/31 11:45:58by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

server::server() : _nbClient(0), _nbChannel(0)
{
    name = ":localserver";
    _required = false;
    _userinfo = false;
    _isPass = false;
    _pass = "1234";
}

server::~server()
{
    close(_socketFd);
    for (int i = 0; i < _nfds; i++)
    {
        close(_fds[i].fd);
        _fds[i] = _fds[--_nfds];
    }
    std::cout << "Server destructor" << std::endl;
}

void    server::initServer()
{
    _sa.sin_family = AF_INET;
    _sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    _sa.sin_port = htons(PORT);
}

void    server::initSocket()
{
    _socketFd = socket(_sa.sin_family, SOCK_STREAM, 0);
    if (_socketFd == -1) 
    {
        initError ex("socket fd");
        throw ex;
    }
}

void    server::initBind()
{
    _status = bind(_socketFd, (struct sockaddr *)&_sa, sizeof(_sa));
    if (_status != 0)
    {
        std::cerr << "Error on bind: " << strerror(errno) << std::endl;
        close(_socketFd);
        initError ex("bind");
        throw ex;
    }
}

void    server::initListen()
{
    _status = listen(_socketFd, 20);
    if (_status != 0)
    {
        close(_socketFd);
        initError ex("listen");
        throw ex;
    }
}

// void    server::initPoll()
// {
//     struct pollfd serverPollFd;
//     serverPollFd.fd = _socketFd;
//     serverPollFd.events = POLLIN;
//     _pollFds.push_back(serverPollFd);
// }
void    server::closeFd(int clientFd)
{
    close(clientFd);
    for (int i = 0; i < _nfds; i++)
    {
        if (_fds[i].fd == clientFd)
        {
            // _pollFds.erase(_pollFds.begin() + i);
            close(_fds[i].fd);
            break;
        }
    }
    for (int i = 1; i <= _nbClient; i++)
    {
        if (_userN[i]->getClientFd() == clientFd)
        {
            delete _userN[i];
            for(int j = i; j < _nbClient - 1; j++)
                _userN[j] = _userN[j + 1];
            _nbClient--;
            break;
        }
    }
}
void    server::quit(user * user)
{
    std::cout << "Now closing client : " << user->getUsername() << std::endl;
    close(user->getClientFd());
    for (int i = 0; i < _nfds; i++)
        {
            if (_fds[i].fd == user->getClientFd())
            {
                // _fds.erase(_pollFds.begin() + i);
                std::cout << "to do !! " << std::endl;
                break;
            }
        }
        for (int i = 1; i <= _nbClient; i++)
        {
            if (_userN[i]->getClientFd() == user->getClientFd())
            {
                delete _userN[i];
                for(int j = i; j < _nbClient - 1; j++)
                    _userN[j] = _userN[j + 1];
                _nbClient--;
                break;
            }
        }
}
void    server::sendForInfo(int clientFd, std::string message)
{
    int msgSize = message.size();
    int bytesSend = send(clientFd, message.c_str(), msgSize, 0);
    if (bytesSend == -1)
    {
        initError ex("send");
        throw ex;
    }
    else if (bytesSend != msgSize)
    {
        std::cout << "Only partial message received by client socket : " << clientFd
        << ", bytes send = " << bytesSend << std::endl;
    }
}

void    server::SendSpeMsg(user * userId, user * toSend, std::string msg)
{
    std::string sep = " ";
    std::string userMsg = ":" + userId->getNick() + "!" + userId->getUsername() + "@" + userId->getHostname() + sep + msg;
    int msgSize = userMsg.size();
    int bytesSend = send(toSend->getClientFd(), userMsg.c_str(), msgSize, 0);
    if (bytesSend == -1)
    {
        initError ex("send");
        throw ex;
    }
    else if (bytesSend != msgSize)
    {
        std::cout << "Only partial message received by client socket : " << toSend->getClientFd()
        << ", bytes send = " << bytesSend << std::endl;
    }
}

void    server::sendMessage(user * user, int numCode, std::string message)
{
    std::string userMsg;
    if (numCode != NOCODE)
    {
        std::string sep = " ";
        userMsg = name + sep + toStr(numCode) + sep + user->getNick() + sep + message + "\r\n";
    }
    else
    {
        userMsg = message + "\r\n";
    }
    int msgSize = userMsg.size();
    int bytesSend = send(user->getClientFd(), userMsg.c_str(), msgSize, 0);
    if (bytesSend == -1)
    {
        initError ex("send");
        throw ex;
    }
    else if (bytesSend != msgSize)
    {
        std::cout << "Only partial message received by client socket : " << _clientFd
        << ", bytes send = " << bytesSend << std::endl;
    }
}
void    server::manageInput(user * user, std::string input)
{
    parsingCommand(input);
    if (_command[0] == "PING")
        sendMessage(user, NOCODE, "PONG");
    int i = 0;
    std::string call[7] = {"/NICK", "/USER", "/JOIN", "/MSG", "KICK", "INVITE", "TOPIC"};
    void (user::*ptr[7])() = {&user::nick, &user::userName, &user::join, &user::msg, &user::kick, &user::invite, &user::defTopic};
    while (i < 7 && _command[0] != call[i])
        i++;
    if (i < 7)
        (user->*ptr[i])();
    else
        msgToCurrent(user, input);
}

void   server::onlyOne(user * user, std::string input)
{
    if (input == "/QUIT")
        quit(user);
    if (input == "channel")
        printChannelInfo();
    if (input == "user")
        user->checkUserChannelList();
    if(input == "alluser")
        printInfoUsers();
    int i = 0;
    std::string call[6] = {"/INFO", "/HELP", "/WHO", "/LIST", "/PART", "TOPIC"};
    void (user::*ptr[6])() = {&user::info, &user::help, &user::who, &user::list, &user::leave, &user::topic};
    while (i < 6 && input != call[i])
        i++;
    if (i < 6)
        (user->*ptr[i])();
    else
        msgToCurrent(user, input);
}
void    server::msgToCurrent(user * user, std::string input)
{
    if (!user->getInChannel())
        sendMessage(user, NOCODE, "Currently you aren't in any channel !\r\n");
    else
    {
        std::string channelName = user->getCurrChannel();
        channel *curr = user->getChannelByName(channelName);
        sendMsgToChanFromUser(curr, user, input);
    }
}

void    server::sendMsgToChanFromUser(channel * channel, user * user, std::string input)
{
    int i = 1;
    while (i <= channel->getNbUser())
    {
        std::string from = user->getNick();
        std::string msg = "<" + from + "> " + input + "\r\n";
        sendMessage(channel->getUserN(i), NOCODE, msg);
        i++;
    }
}

void    server::parsingMsg(user * user, std::string input)
{
    size_t sepa = 0;
    sepa = input.find(" ");
    if (sepa != std::string::npos)
        manageInput(user, input);
    else
        onlyOne(user, input);
}

void    server::parsingCommand(std::string input)
{
    input = trim_and_reduce_spaces(input);
    size_t start = 0;
    size_t end = input.find(" ");
    _command.clear();
    while (end != std::string::npos)
    {
        _command.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(" ", start);
    }
    _command.push_back(input.substr(start));
}
std::vector<std::string>    server::parsingIntoVector(std::string input)
{
    std::vector<std::string> command;
    input = trim_and_reduce_spaces(input);
    size_t start = 0;
    size_t end = input.find(" ");
    while (end != std::string::npos)
    {
        command.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(" ", start);
    }
    command.push_back(input.substr(start));
    return (command);
}
void    server::manageMsg(int clientFd, std::string input)
{
    user* currUser = NULL;
    for(int i = 1; i <= _nbClient; i++)
    {
        if (_userN[i]->getClientFd() == clientFd)
        {
            currUser = _userN[i];
            break;
        }
    }
    if (currUser != NULL)
    {
        parsingMsg(currUser, input);
        if (input != "/QUIT\r\n")
            infoClient(currUser->getClientFd());
    }
    else
        std::cout << "curr User is null !!!!" << std::endl;
}
bool    server::manageUserInfo(int clientFd, std::string input)
{
    std::vector<std::string> command = parsingIntoVector(input);
    // if (command.size() != 5)
        // return (false);
    for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
    {
        if (*it == "USER")
        {
            manageUser(clientFd, command);
        }
    }
    for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
    {
        if (*it == "NICK")
        {
            it++;
            manageNick(clientFd, *it);
            // sendMessage(getUserByFd(clientFd), RPL_NICK, "Nickname register");
        }
    }
    // for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
    // {
    //     if (*it == "LS") 
    //     {
    //         std::string cap_response = (name + " CAP * LS :multi-prefix sasl\r\n");
    //         // sendMessage(getUserByFd(clientFd), NOCODE, cap_response);
    //         sendForInfo(clientFd, cap_response);
    //     }
    // }
    // for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
    // {
    //     if (*it == "CAP REQ :multi-prefix") 
    //     {
    //         std::string cap_require = (name + " CAP * ACK :multi-prefix\r\n");
    //         // sendMessage(getUserByFd(clientFd), NOCODE, cap_response);
    //         sendForInfo(clientFd, cap_require);
    //     }
    // }
        // sendMessage(clientFd, NOCODE, "tamere la grosse pute");
    // // std::string msg = "Welcome to the network, " + getUserByFd(clientFd)->getNick() + "\r\n";
    // sendMessage(getUserByFd(clientFd), RPL_WELCOME, msg);
    printInfoNewUser(getUserByFd(clientFd));
    _idxClient[_nbClient] = _nbClient;
    // infoClient(clientFd); 
    return (true);
    // if (command)
    // if (command[0] == "USER")
    // {
    //     if (manageUser(clientFd, command))
    //     {
    //         sendForInfo(clientFd, "Username : You re now register\r\n"); 
    //         return (true);
    //     }
    //     return (false);
    // }
    // else
    //     return (false);
}
bool    server::manageNick(int clientFd, std::string nickname)
{
    getUserByFd(clientFd)->setNickname(nickname);
    return (true);
}

bool    server::manageUser(int clientFd, std::vector<std::string> command) 
{
    // if (!isValidUsername(command[1]))
    // {
    //     std::string msg = name + " " + toStr(ERR_ERRONEUSNICKNAME) + " * " + ":Invalid " + command[1] + "\r\n";
    //     sendForInfo(clientFd, msg);
    //     return (false);
    // }
    // else
    // {
        user *newUser = new user(*this, clientFd, command);
        _nbClient++;
        newUser->setIdx(_nbClient);
        _userN[_nbClient] = newUser;
        _userinfo = true;
                return (true);
    // }
}
// bool    server::manageUser(int clientFd, std::vector<std::string> command) 
// {
//     // if (!isValidUsername(command[1]))
//     // {
//     //     std::string msg = name + " " + toStr(ERR_ERRONEUSNICKNAME) + " * " + ":Invalid " + command[1] + "\r\n";
//     //     sendForInfo(clientFd, msg);
//     //     return (false);
//     // }
//     // else
//     // {
//         user *newUser = new user(*this, clientFd, command);
//         _nbClient++;
//         newUser->setIdx(_nbClient);
//         _userN[_nbClient] = newUser;
//                 return (true);
//     // }
// }




void    server::infoClient(int clientFd)
{
    std::string info;
    std::string format = "$> ";
    std::string format2 = " ------IRC------[";
    std::string format3 = "]------>";
    user * curr = getUserByFd(clientFd);
    info = format + curr->getNick() + format2
    + curr->getCurrChannel() + format3 + "\r\n";
    int msgSize = info.size();
    int byteS = send(clientFd, info.c_str(), msgSize, 0);
    if (byteS == -1)
    {
        initError ex ("send");
        throw ex;
    }
    else if (byteS != msgSize)
    {
        std::cout << "Only partial message received by client socket : " << _clientFd
        << ", bytes send = " << byteS << std::endl;
    }
}



void    server::readingClientFirst(int clientFd)
{
    std::cout << "reading client first" << std::endl;
    char buff[BUFSIZ] = {0};
    _bytesRead = recv(clientFd, buff, BUFSIZ - 1, 0);
    std::cout << "buffer : " << buff << std::endl;
    std::string input = buff;
    if (_bytesRead == -1)    
    {
        initError ex("recv");
        throw ex;
    }
    else if (_bytesRead == 0)
        closeFd(clientFd);
    else
    {
        buff[_bytesRead] = '\0';
        std::string input(buff);
        std::cout << "reading input else : " << input << std::endl;

        // if (input == "PASS 1234")
        // checkRequiredLS(clientFd, input);
        // else
            // sendForInfo(clientFd, "pass need");
        manageUserInfo(clientFd, input);
    }
}

void    server::readingClient(int clientFd)
{
    std::cout << "reading client" << std::endl;
    char buff[BUFSIZ] = {0};
    _bytesRead = recv(clientFd, buff, BUFSIZ - 1, 0);
    std::cout << "buffer : " << buff << std::endl;
    std::string input = buff;
    if (_bytesRead == -1)    
    {
        initError ex("recv");
        throw ex;
    }
    else if (_bytesRead == 0)
        closeFd(clientFd);
    else
    {
        buff[_bytesRead] = '\0';
        std::string input(buff);
        std::cout << "reading input else : " << input << std::endl;
        // if (_required == false)
            // checkRequiredREQ(clientFd, input);
        // if (_userinfo == false)
            // manageUser(clientFd);
        // else if (_required == true && _userinfo == true)
        if (getUserByFd(clientFd))
            welcome(clientFd);

        manageMsg(clientFd, input);
    }
}

void    server::checkRequiredLS(int clientFd, std::string input)
{
    (void)clientFd;
    std::vector<std::string> command = parsingIntoVector(input);
    for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
    {
        if (*it == "LS") 
        {
            std::string cap_response = (name + " CAP * LS :multi-prefix sasl\r\n");
            sendForInfo(clientFd, cap_response);
        }
        if (*it == "NICK")
        {
            it++;
            _savenick = *it;
        }
        if (*it == "USER")
        {
            it++;
            _saveuser = *it;
            it++;
            it++;
            _savehost = *it;
            it++;
            it++;
            _savereal = *it;
        }
    }
}

void    server::checkRequiredREQ(int clientFd, std::string input)
{
    std::vector<std::string> command = parsingIntoVector(input);
    for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
    {
        if (*it == "REQ") 
        {
            std::string cap_require = (name + " CAP * ACK :multi-prefix\r\n");
            _required = true;
            sendForInfo(clientFd, cap_require);
        }
    }
}

void    server::welcome(int clientFd)
{
    std::string nick = getUserByFd(clientFd)->getNick();
    std::string user = getUserByFd(clientFd)->getUsername();
    std::string host = getUserByFd(clientFd)->getHostname();
    
    sendMessage(getUserByFd(clientFd), RPL_WELCOME, (":Welcome to myIrc " + nick + "!" + user + "@" + host));
    sendMessage(getUserByFd(clientFd), RPL_YOURHOST, (":Your host is " + name + " running version 1.0"));
    sendMessage(getUserByFd(clientFd), RPL_CREATED, ":This server was created Tue Aug 20 2024");
    sendMessage(getUserByFd(clientFd), RPL_MYINFO, (name + " 1.0 " + "iwso ov"));
    // sendMessage(getUserByFd(clientFd), RPL_NICK, "Nickname register");
    sendMessage(getUserByFd(clientFd), RPL_ENDOFMOTD, ":End of /MOTD command");
}

void server::handleClient(int clientFd) {
    const char *welcomeMsg = ":irc.example.com 001 Welcome to the Internet Relay Network\r\n";
    send(clientFd, welcomeMsg, strlen(welcomeMsg), 0);
    
    // Additional IRC protocol messages can be sent here
}

void server::waitingClient() 
{
    _nfds = 1;
    _fds[0].fd = _socketFd;
    _fds[0].events = POLLIN;

    while (true)
    {
        int pollCount = poll(_fds, _nfds, -1);
        if (pollCount == -1)
        {
            std::cerr << "Error on poll: " << strerror(errno) << std::endl;
            break;
        }
        for (int i = 0; i < _nfds; ++i)
        {
            if (_fds[i].revents & POLLIN) 
            {
                if (_fds[i].fd == _socketFd) 
                {
                    int clientFd = accept(_socketFd, NULL, NULL);
                    if (clientFd == -1) {
                        std::cerr << "Error on accept: " << strerror(errno) << std::endl;
                        continue;
                    }
                    std::cout << "New client connected: " << clientFd << std::endl;
                    handleClient(clientFd);
                    welcome(clientFd);
                    if (_nfds < MAXCLIENT)
                    {
                        _fds[_nfds].fd = clientFd;
                        _fds[_nfds].events = POLLIN;
                        _nfds++;
                    }
                    else
                    {
                        std::cerr << "Max clients reached, closing connection: " << clientFd << std::endl;
                        close(clientFd);
                    }
                }
                else
                {
                    // readingClient2()
                    char buffer[512];
                    int bytesRead = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
                    if (bytesRead <= 0)
                    {
                        if (bytesRead == 0)
                            std::cout << "Client disconnected: " << _fds[i].fd << std::endl;
                        else 
                            std::cerr << "Error on recv: " << strerror(errno) << std::endl;
                        close(_fds[i].fd);
                        _fds[i] = _fds[--_nfds];
                    }
                    else 
                        std::cout << "Received message from client " << _fds[i].fd << ": " << std::string(buffer, bytesRead) << std::endl;
                }
            }
        }
    }
}
// void    server::handleClient(int clientFd)
// {
//     std::cout << "handle client" << std::endl;
//     if (_nbClient >= MAXCLIENT)
//     {
//         std::cerr << "Max client reach" << std::endl;
//         return ;
//     }
//     struct pollfd clientPollFd;
//     clientPollFd.fd = clientFd;
//     clientPollFd.events = POLLIN;
//     _pollFds.push_back(clientPollFd);
//     _clientFd = clientFd;
//     // sendForInfo(clientFd, "Pass word need ");

//     readingClientFirst(clientFd);
// }
// void    server::waitingClient()
// {
//     std::cout << "Waiting for client connection" << std::endl << std::endl;
//     while (true) 
//     {
//         int pollCount = poll(&_pollFds[0], _pollFds.size(), -1);
//         if (pollCount == -1)
//         {
//             initError ex("poll");
//             throw ex;
//         }
//         for (size_t i = 0; i < _pollFds.size(); i++)
//         {
//             if (_pollFds[i].revents & POLLIN)
//             {
//                 if (_pollFds[i].fd == _socketFd)
//                 {
//                     std::cout << "new client trying" << std::endl;
//                     _clientFd = accept(_socketFd, (struct sockaddr *)&_clienAddr, &_addrSize);
//                     if (_clientFd == -1)
//                     {
//                         initError ex("accept");
//                         throw ex;
//                     }
//                     std::cout << "New client connected, fd : " << _clientFd << std::endl;
//                     int flag = 1;
//                     if (setsockopt(_clientFd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int)) < 0)
//                     {
//                         std::cerr << "Error TCP" << std::endl;
//                         perror("setsockopt failed");
//                     }
//                     handleClient(_clientFd);
//                 }
//                 else
//                 {
//                     readingClient(_pollFds[i].fd);
//                 }
//             }
//         }
//     }
// }

void    server::run()
{
    try
    {
        initServer();
        initSocket();
        initBind();

        std::cout << "[ SERVER RUN ]" << std::endl;
        std::cout << "Connection socket to localhost, PORT : " << PORT << std::endl;
        std::cout << std::endl;

        initListen();
        // initPoll();
        
        _addrSize = sizeof(_clienAddr);
        
        waitingClient();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void    server::delUserList(user * user)
{
    if (_loginClient.size() == 1)
        _loginClient.pop_back();
    else
    {
        for (std::vector<std::string>::iterator it = _loginClient.begin(); it != _loginClient.end(); it++)
        {
            if (*it == user->getUsername())
            {
                _loginClient.erase(it);
                break;
            }
        }
    }
    if (nicknameClient.size() == 1)
        nicknameClient.pop_back();
    else
    {
        for (std::vector<std::string>::iterator it = nicknameClient.begin(); it != nicknameClient.end(); it++)
        {
            if (*it == user->getNick())
            {
                nicknameClient.erase(it);
                break;
            }
        }
    }
}


void    server::updateNicknameList(std::string old, std::string nickname)
{
    if (nicknameClient .size() == 1)
        nicknameClient[0] = nickname;
    else
    {
        for (std::vector<std::string>::iterator it = nicknameClient.begin(); it != nicknameClient.end(); it++)
        {
            if (*it == old)
            {
                nicknameClient.erase(it);
                nicknameClient.push_back(nickname);
                break;
            }
        }
    }
}
void    server::updateLoginList(std::string old, std::string login)
{
    if (_loginClient.size() == 1)
        _loginClient[0] = login;
    else
    {
        for (std::vector<std::string>::iterator it = _loginClient.begin(); it != _loginClient.end(); it++)
        {
            if (*it == old)
            {
                _loginClient.erase(it);
                _loginClient.push_back(login);
                break;
            }
        }
    }
}
void   server::checkChannel(std::string currChannel)
{
    if (_nbChannel > 0)
    {
        for (int i = 1; i <= _nbChannel; i++)
        {
            if (channelId[i]->getName() == currChannel)
            {
                if (channelId[i]->getNbUser() == 0)
                {
                    std::cout << "Channel : " << currChannel << " have been delete." << std::endl;
                    channelId[i]->~channel();
                    delete channelId[i];
                    _nbChannel--;
                }
            }
        }
    }
}

bool    server::isValidNickname(std::string nickname)
{
    for (std::vector<std::string>::iterator it = nicknameClient.begin(); it != nicknameClient.end(); it++)
    {
        if (*it == nickname)
            return (false);
    }
    if (!isalpha(nickname[0]) || nickname[0] == '@')
        return (false);
    return (true);
}
bool    server::isValidUsername(std::string username)
{
    if (!isalpha(username[0]))
        return (false);
    return (true);
}
int     server::getFdClientByNick(std::string nickname)
{
    int fd;
    for (int i = 1; i <= _nbClient; i++)
    {
        if (_userN[i]->getNick()[0] == '@')
        {
            std::string majnick = _userN[i]->getNick().erase(0, 1);
            if (majnick == nickname)
            {
                fd = _userN[i]->getClientFd();
                break ;
            }
        }
        if (_userN[i]->getNick() == nickname)
        {
            fd = _userN[i]->getClientFd();
            break ;
        }
    }
    return (fd);
}

user*  server::getUserByFd(int clientFd)
{
    for (int i = 1; i <= _nbClient; i++)
    {
        if (_userN[i] == NULL)
            return (NULL);
        if (_userN[i]->getClientFd() == clientFd)
            return (_userN[i]);
    }
    return (NULL);
}

user*   server::getUserByNickname(std::string nickname)
{
    std::vector<std::string> list = nicknameClient;
    int i = 1;
    for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); it++)
    {
        if (nickname == *it)
        {
            return (_userN[i]);
        }
        i++;
    }
    return (NULL);
}

int     server::getNbClient(void)
{
    return (_nbClient);
}
int     server::getNbChannel(void)
{
    return (_nbChannel);
}
std::vector<std::string>   server::getCommand(void)
{
    return (_command);
}

std::vector<std::string>  server::getLogin()
{
    return (_loginClient);
}

void    server::setLogin(std::string login)
{
    _loginClient.push_back(login);
}

int     server::getSocketfd()
{
    return (_socketFd);
}

void    server::setNbChannel(int i)
{
    _nbChannel += i;
}
void    server::printLoginList()
{
    std::vector<std::string> tmp = _loginClient;
    for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
    {
        std::cout << *it << std::endl;
    }
}

void   server::printInfoNewUser(user *user)
{
    std::cout << "there is a new client -> " << std::endl;
    std::cout << "Socket client : " <<  user->getClientFd() << std::endl
            << "Username client : "  << user->getUsername() << std::endl 
            << "Nickname client : " << user->getNick() << std::endl << std::endl;
}

void   server::printInfoUsers()
{
    for (int i = 1; i <= _nbClient; i++)
    {
        std::cout << "Socket client : " << _userN[i]->getClientFd() << std::endl
            << "Login client : "  << _userN[i]->getUsername() << std::endl 
            << "Nickname client : " << _userN[i]->getNick() << std::endl 
            << "idx user in server : " << _idxClient[i] << std::endl;
    }
}
void    server::printCmd(std::vector<std::string> command)
{
    for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
        std::cout << *it << std::endl;
}
void    server::printChannelInfoByChannel(std::string channelName)
{
    std::cout << "printf channel info userN" << std::endl;
    
    if (_nbChannel == 0)
        std::cout << "Actually no channel !" << std::endl;
    else
    {
        std::cout << "each channel info : " << std::endl;
        for (int i = 1; i <= _nbChannel; i++)
        {
            std::cout << "Channel name : " << channelId[i]->getName() << std::endl;
            std::cout << "Channel nb user : " << channelId[i]->getNbUser() << std::endl;
            for (int j = 1; j <= channelId[i]->getNbUser(); j++)
            {  
                std::cout << "-----------" << std::endl;
                std::string name = channelId[i]->getUserN(j)->getNick();
                int idx = channelId[i]->getIdxUserByNickname(name);
                std::cout << "name user in channel : " << name << std::endl;
                std::cout << "idx user in channel : " << idx << std::endl;
                std::cout << "-----------" << std::endl;
            }
        }
    }
    (void)channelName;
}
void    server::printChannelInfo()
{
    std::cout << "printf channel info channelId" << std::endl;
    if (_nbChannel == 0)
        std::cout << "Actually no channel !" << std::endl;
    else
    {
        std::cout << "each channel info : " << std::endl;
        for (int i = 1; i <= _nbChannel; i++)
        {
            std::cout << "Channel name : " << channelId[i]->getName() << std::endl;
            std::cout << "Channel nb user : " << channelId[i]->getNbUser() << std::endl;
            for (int j = 1; j <= channelId[i]->getNbUser(); j++)
            {  
                std::cout << "-----------" << std::endl;
                std::string name = channelId[i]->getUserN(j)->getNick();
                int idx = channelId[i]->getIdxUserByNickname(name);
                std::cout << "name user in channel : " << name << std::endl;
                std::cout << "idx user in channel : " << idx << std::endl;
                std::cout << "-----------" << std::endl;
            }
        }
    }
}
server::initError::initError(const std::string& error)
{
    std::strncpy(_error, "Error on : ", 99);
    std::strncat(_error, error.c_str(), 99 - std::strlen(_error));
    _error[99] = '\0';
}

const char* server::initError::what() const throw()
{
    return (_error);
}
// void    server::receptInfo(std::string input, int clientFd)
// {
//     std::cout << " recept info " << std::endl;
//     if (input == "HELP")
//         sendForInfo(clientFd, "USER [username] [hostname(unused)] [servername(unused)] [realname]\n      NICK [nickname]\r\n");
//     else
//     {
//         if (!getUserByFd(clientFd))
//         {
//             if (!manageUserInfo(clientFd, input))
//             {
//                 std::cout << "info require in recept info " << std::endl;
//                 infoRequired(clientFd);
//                 return ;
//             }
//         }
//         else if (!manageNickInfo(clientFd, input))
//             infoRequired(clientFd);
//         else if (getUserByFd(clientFd) != NULL && getUserByFd(clientFd)->getNick().empty())
//             infoRequired(clientFd);
//         else
//         {
//             // std::string msg = "Welcome to the network, " + getUserByFd(clientFd)->getNick() + "\r\n";
//             // sendMessage(getUserByFd(clientFd), RPL_WELCOME, msg);
//             printInfoNewUser(getUserByFd(clientFd));
//             _idxClient[_nbClient] = _nbClient;
//             infoClient(clientFd); 
//         }
//     }
// }

// void    server::infoRequired(int clientFd)
// {
//     std::cout << "info required" << std::endl;
//     if (!getUserByFd(clientFd))
//     {
//         sendForInfo(clientFd, "Info Required : USER and NICK (type HELP for usage)\r\n");
//         return ;
//     }
//     else if (getUserByFd(clientFd)->getNick().empty())
//         sendForInfo(clientFd, "Info required : NICK\r\n");
// }
// bool        server::manageNick(int clientFd, std::string nickname)
// {
    // if (!isValidNickname(nickname))
    // {
    //     std::string msg = name + " " + toStr(ERR_ERRONEUSNICKNAME) + " * " + ":Invalid " + nickname + "\r\n"; 
    //     sendForInfo(clientFd, msg);
    //     return (false);
    // }
    // else
    // {
//         if (getUserByFd(clientFd))
//         {
//             getUserByFd(clientFd)->setNickname(nickname);
//             nicknameClient.push_back(nickname); 

//         }
//         else
//             std::cout << "error on user/nick" << std::endl;
//         return (true);
//}

/// bool    server::manageUserInfo(int clientFd, std::string input)
// {
//     std::vector<std::string> command = parsingIntoVector(input);
//     if (command.size() != 5)
//         return (false);
//     else if (command[0] == "USER")
//     {
//         if (manageUser(clientFd, command))
//         {
//             sendForInfo(clientFd, "Username : You re now register\r\n"); 
//             return (true);
//         }
//         return (false);
//     }
//     else
//         return (false);
// }

// bool    server::manageNickInfo(int clientFd, std::string input)
// {
//     std::vector<std::string> command = parsingIntoVector(input);
//     if (command.size() != 2)
//         return (false);
//     else if (command[0] == "NICK")
//     {
//         if (manageNick(clientFd, command[1]))
//         {
//             sendForInfo(clientFd, "Nickname : You re now register\r\n"); 
//             return (true);
//         }
//         return (false);
//     }
//     else
//         return (false);
// }/ }