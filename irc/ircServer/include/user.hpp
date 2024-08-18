/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:57:15 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/18 16:30:06 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "main.hpp"

class server;
class channel;

class user
{
    private:

                server&                 _server;
                int                     _clientFd;
                bool                    _inChannel;
                int                     _idx;
                std::string             _realName;
                std::string             _username;
                std::string             _nickname;
                std::string             _hostname;
                std::string             _currChannel;
                bool                    _clientRegister;

    public:

                user(server & srv, int clientFd, std::vector<std::string> command);
                ~user();


                

                bool            isValidNickname(std::string nickname);
                bool            isValidUsername(std::string username);
                bool            isValidChannelName(std::string name);
                bool            checkNicknameList(std::string nickname);
                bool            inviteCommandCheck(std::string channel);
                bool            topicCommandCheck(std::string channel);
                int             checkChannel(void);
                bool            checkUser(void);
                bool            checkKickInfo(void);
                bool            checkUserChannelList(void);
                user*           findUserChannelByName(std::string name);

                void            userName(void);
                void            join(void);
                void            nick(void);
                void            msg(void);
                void            kick(void);
                void            invite(void);
                void            topic(void);
 
                void            info(void);
                void            who(void);
                void            list(void);
                void            quit(void);
                void            help(void);
                void            leave(void);

                void            speCommandOp(void);
                
                int             getClientFd(void);
                int             getIdx(void);
                std::string     getUsername(void);
                std::string     getNick(void);
                std::string     getCurrChannel(void);
                std::string     getHostname(void);
                std::string     getReal();
                bool            getInChannel(void);
                // bool            getOpChannel(void);

                void            setIdx(int idx);
                void            decrementIdx(void);
                // void            setOpchannel(bool op);
                void            setNickname(std::string nickname);
                void            registerChannel(std::string name, channel * channel);
                int             checkChannel2(std::string name);
                
                channel*        channelUser[MAXCHANNEL];
                channel*        getChannelByIdx(int idx);
                channel*        getChannelByName(std::string name); 
                void            createNewChannel(std::string name);
                void            joinChannel(std::string name);

                class NotValidUserName : public std::exception
                {
                        virtual const char* what() const throw();
                };

                class NotValidNickName : public std::exception
                {
                        virtual const char* what() const throw();       
                };

                class NotValidChannelName : public std::exception
                {
                        virtual const char* what() const throw();
                };
};

// template <typename T> std::string toStr(T tmp)
// {
//     std::ostringstream out;
//     out << tmp;
//     return out.str();
// }

#endif