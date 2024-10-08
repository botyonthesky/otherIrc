/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:57:15 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/27 14:58:38 by tmaillar         ###   ########.fr       */
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
                bool                    _isOperator;

    public:

                user(server & srv, int clientFd, std::vector<std::string> command);
                ~user();


                
                void            addInfo(std::vector<std::string> command);
                void            processUser(std::vector<std::string> command, size_t& i);
                void            processNick(std::vector<std::string> command, size_t& i);
                std::string     extractRealName(std::vector<std::string> info);

                bool            isValidNickname(std::string nickname);
                bool            isValidUsername(std::string username);
                bool            isValidChannelName(std::string name);
                bool            checkNicknameList(std::string nickname);
                bool            inviteCommandCheck(std::string channel);
                bool            topicCommandCheck(std::string channel);
                int             checkChannel(void);
                bool            checkUser(void);
                bool            checkKickInfo(void);
                bool            checkUserChannelList(std::string nickname);
                user*           findUserChannelByName(std::string name);

                void            userName(void);
                void            join(void);
                void            nick(void);
                void            msg(void);
                void            kick(void);
                void            invite(void);
                void            topic(void);
                void            defTopic(void);
                void            mode(void);
                void            ping(void);
                void            privmsg(void);

 
                void            info(void);
                void            who(void);
                void            whois(void);
                void            list(void);
                void            quit(void);
                void            help(void);
                void            leave(void);

                void            speCommandOp(void);
                std::string     mergeCommand(std::vector<std::string> command);


                channel*        channelUser[MAXCHANNEL];
                channel*        getChannelByIdx(int idx);
                channel*        getChannelByName(std::string name); 
                void            createNewChannel(std::string name);
                void            joinChannel(std::string name);
                void            registerChannel(std::string name, channel * channel);
                int             checkChannel2(std::string name);
                int             privOrChan(std::string input);
                bool            isValidChannel(std::string name);
                bool            isValidAttribut(std::string attribut);
                bool            isValidArgv(std::string attribut, std::vector<std::string> command);
                
                int             getClientFd(void);
                int             getIdx(void);
                std::string     getUsername(void);
                std::string     getNick(void);
                std::string     getCurrChannel(void);
                std::string     getHostname(void);
                std::string     getReal(void);
                server&         getServer(void);
                bool            getInChannel(void);
                bool            getIsOpe(void);                        

                void            setIsOpe(bool value);
                void            setIdx(int idx);
                void            decrementIdx(void);
                void            setNickname(std::string nickname);
               
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

#endif