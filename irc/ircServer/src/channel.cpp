/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:18:47 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/27 15:32:26 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

channel::channel(server & srv, user * user, std::string name) : _server(srv), _nbUsers(0)
{
    try
    {
        _name = name;
        _nbUsers++;
        _idxUser = _nbUsers;
        _userN[_idxUser] = user;
        _nameOperator = user->getNick();
        _onInvitOnly = false;
        _topicForOpOnly = false;
        _onPassWordOnly = false;
        topic = "General";
        timestamp();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
void	channel::timestamp()
{
    std::string real_time;
    std::time_t now = std::time(0);
    createTime = toStr(now);
} 
channel::~channel()
{
}

std::string     channel::getName()
{
    return (_name);
}
int         channel::getIdxUser(void)
{
    return (_idxUser);
}
void        channel::setIdxUser(int idx)
{
    _idxUser = idx;
}
int         channel::getIdx(void)
{
    return (_idx);
}
void        channel::setIdx(int idx)
{
    _idx = idx;
}
int         channel::getNbUser()
{
    return (_nbUsers);
}

user*       channel::getUserN(int idx)
{
   return (_userN[idx]);
}
void        channel::setNbUser(int x)
{
    _nbUsers += x;
}

void        channel::setUserN(user * user, int idx)
{
    _userN[idx] = user;
}

void        channel::delUserN(int idx)
{
    if (idx < 1 || idx > _nbUsers)
    {
        std::cout << "Invalid idx" << std::endl;
    }
    else
    {
        for (int i = idx; i < _nbUsers; i++)
            _userN[i] = _userN[i + 1];
        _userN[_nbUsers] = NULL;
        _nbUsers--;
    }
}

int     channel::getIdxUserByNickname(std::string nickname)
{
    for (int i = 1; i <= getNbUser(); i++)
    {
        if (_userN[i]->getNick() == nickname)
            return (i);
    }
    return (-1);
}

void    channel::majIdxUserChannel()
{
    int i = 1;
    while(_userN[i])
    {
        if (_userN[i]->getIdx() > _nbUsers)
        {
            _userN[i]->setIdx(-1);
        }
        else
            i++;      
    }
        
}

void    channel::applyMode(std::string attribute)
{
    std::string call[10] = {"+i", "-i", "+t", "-t", "+k", "-k", "+o", "-o", "+l", "-l"};
    void (channel::*ptr[10])() = {&channel::modei, &channel::modei,
    &channel::modet, &channel::modet, &channel::modek, &channel::modek,
    &channel::modeo, &channel::modeo, &channel::model, &channel::model};
    int i = 0;
    while (i < 10 && attribute != call[i])
        i++;
    if (i < 10)
        (this->*ptr[i])();
    return ;
}

void    channel::modei()
{
    std::cout << "modei" << std::endl;
    std::string attribut = _server.getCommand()[2];
    if (attribut[0] == '+')
        this->_onInvitOnly = true;
    else if (attribut[0] == '-')
        this->_onInvitOnly = false;
}

void    channel::modet()
{
    std::cout << "modet" << std::endl;   
    std::string attribut = _server.getCommand()[2];
    if (attribut[0] == '+')
        this->_topicForOpOnly = true;
    else if(attribut[0] == '-')
        this->_topicForOpOnly = false;
   
}

void    channel::modek()
{
    std::cout << "modek" << std::endl;
    std::string attribut = _server.getCommand()[2];
    std::string pass = _server.getCommand()[3];
    if (attribut[0] == '+')
    {
        _onPassWordOnly = true;
        _password = pass;
    }
    else if (attribut[0] == '-')
    {
        _onPassWordOnly = false;
    }

}

void    channel::modeo()
{
    std::cout << "modeo" << std::endl;
    std::string attribut = _server.getCommand()[2];
    std::string nickname = _server.getCommand()[3];
    if (attribut[0] == '+')
    {
        for (int i = 1; i <= _nbUsers; i++)
        {
            if (_userN[i]->getNick() == nickname)
                _userN[i]->setIsOpe(true);
        }
    }
    else if (attribut[0] == '-')
    {
        for (int i = 1; i <= _nbUsers; i++)
        {
            if (_userN[i]->getNick() == nickname)
                _userN[i]->setIsOpe(false);
        }
    }
}


void    channel::model()
{
    std::cout << "model" << std::endl;
    std::string attribut = _server.getCommand()[2];
    if (attribut[0] == '+')
    {
        std::string nbs = _server.getCommand()[3];
        char *end;
        long nb = std::strtol(nbs.c_str(), &end, 10);
        _maxUserChannel = true;
        _nbClientChannel = nb;
    }
    if (attribut[0] == '-')
    {
        _maxUserChannel = false;
    }

}

std::string    channel::getNameOperator()
{
    return (_nameOperator);
}


