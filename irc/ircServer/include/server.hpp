/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:37:25 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/26 15:24:03 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../include/main.hpp"
#include <fcntl.h>

#define MAXCLIENT 10
#define MAXCHANNEL 3
#define PORT 6667

class user;
class channel;

class server
{
    private:
        
        struct sockaddr_in          _sa;
        struct sockaddr_storage     _clienAddr;
        socklen_t                   _addrSize;
        int                         _socketFd;
        int                         _clientFd;
        int                         _status;
        int                         _bytesRead;
        struct pollfd               _fds[MAXCLIENT];
        int                         _nfds;
        
        std::string                 _port;
        std::string                 _password;
        std::string                 _createTime;
        
        bool                        _checkPass;
        bool                        _userinfo;
        bool                        _required;

        int                         _nbClient;
        int                         _nbChannel;

        int                                     _idxClient[MAXCLIENT];
        user*                                   _userN[MAXCLIENT];
        std::vector<std::string>                _loginClient;
        std::vector<std::string>                _command; 
        std::map<int, std::string>              _partialMessages;
        std::vector<std::string>                _infoUser;
         
    public:
        
        server(std::string port, std::string password);
        ~server();

        channel*                    channelId[MAXCHANNEL];
        std::vector<std::string>    nicknameClient;
        std::string                 name;


        void    run(void);
        void    initServer(void);
        void    initSocket(void);
        void    initBind(void);
        void    initListen(void);

        void    closeFd(int clientFd);
        void    infoRegister(std::string input, int i);

        bool    checkPassword(int clientfd, std::string input);
        void    recepMsg(std::string input, int clientFd, bool info);
        void    waitingClient(void);
        void    readingClient(int i);
        void    manageBuff(std::string input, int i);
        bool    manageUserInfo(int clientFd, std::string input);
        bool    manageNickInfo(int clienFd, std::string input);
        bool    manageUser(int clientFd, std::string input);
        bool    manageNick(int clientFd, std::string nickname);

        bool    isValidUsername(std::string username);
        bool    isValidNickname(std::string nickname);
        bool    checkNick(std::string nickInfo, int clientFd);

        void    infoRequired(int clientFd);
        void    handleClient(int clientFd);
        void    infoClient(int clientFd);
        void    sendForInfo(int clientFd, std::string code, std::string message);
        void    receptInfo(std::string input, int clientFd);
        void    readingInfo(int clientFd);

        user*   getUserByFd(int clientFd); 
        user*   getUserN(int idx);
        void    onlyOne(user * user, std::string input);
        void    manageMsg(int clientFd, std::string input);
        void    sendMessage(user * user, std::string numCode, std::string message);
        void    readingClientFirst(int clientFd);
        void    checkRequired(int clientFd, std::string input);
        void	timestamp();
        void    welcome(int clientFd);

        void    SendSpeMsg(user * userId, user * toSend, std::string msg);
        void    groupMsg(user * userId, std::string code, channel * channelId, std::string msg);

        void    parsingMsg(user * user, std::string input);
        void    manageInput(user * user, std::string input);
        void    msgToCurrent(user * user, std::string input);
       
        void    updateLoginList(std::string old, std::string login);
        void    updateNicknameList(std::string old, std::string nickname);

        void    printLoginList(void);
        void    delUserList(user * userId);
        void    removeUser(int clientFd);
        void    sendMsgToChanFromUser(channel * channel, user * user, std::string input);
        int     getFdClientByNick(std::string nickname);
        
        void    quit(user *user);

        void    parsingCommand(std::string input);
        void    printCmd(std::vector<std::string> command);

        void    printInfoNewUser(user *user);
        void    printInfoUsers(void);

        std::vector<std::string>    parsingIntoVector(std::string input);

        void                        checkChannel(std::string currChannel);        
        void                        printChannelInfo(void);
        void                        printChannelInfoByChannel(std::string channelName);

        void                        getNbUserInChannel(std::string currChannel);
        user*                       getUserByNickname(std::string nickname);

        int                         getNbClient(void);
        int                         getSocketfd(void);
        int                         getNbChannel(void);
        std::vector<std::string>    getCommand(void);
        std::vector<std::string>    getLogin(void);

        void                        setLogin(std::string login);
        void                        setNbChannel(int nb);


        
        class initError : public std::exception
        {
            public : 
                explicit initError(const std::string& error);
                virtual const char* what() const throw();
            
            private : 
                char       _error[100];
        };
};


template <typename T> std::string toStr(T tmp)
{
    std::ostringstream out;
    out << tmp;
    return out.str();
}

#endif