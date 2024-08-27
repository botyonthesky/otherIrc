/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:16:40 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/27 15:31:25 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "main.hpp"

class user;
class server;

class channel
{
    private:
        
        server&             _server;
        std::string         _name;
        int                 _idx;
        int                 _idxUser;
        int                 _nbUsers;
        user*               _userN[MAXCLIENT];
        std::string         _nameOperator;
        bool                _onInvitOnly;
        bool                _topicForOpOnly;
        bool                _onPassWordOnly;
        bool                _maxUserChannel;
        int                 _nbClientChannel;
        
        std::string         _password;

    public:
        
        channel(server & srv, user * user, std::string name);
        ~channel();


        std::string     topic;
        std::string     createTime;

        bool            isValidChannelName(std::string name);
        void            majIdxUserChannel(void);
        void	        timestamp(void);
        void            applyMode(std::string attribute);

        void            modei(void);
        void            modet(void);
        void            modek(void);
        void            modeo(void);
        void            model(void);

        std::string     getName(void);
        int             getIdx(void);
        int             getIdxUser(void);
        int             getNbUser(void);
        user*           getUserN(int idx);
        int             getIdxUserByNickname(std::string name);
        std::string     getNameOperator(void);

        void            setIdxUser(int idx);
        void            setIdx(int idx);
        void            setNbUser(int x);
        void            setUserN(user * user, int idx);
        void            setNameOperator(std::string name);
        void            delUserN(int idx);
        
};






#endif