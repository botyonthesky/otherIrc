/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:18:47 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/26 10:20:59 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

channel::channel(user * user, std::string name) : _nbUsers(0)
{
    try
    {
        _name = name;
        _nbUsers++;
        _idxUser = _nbUsers;
        _userN[_idxUser] = user;
        _nameOperator = user->getNick();

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
std::string    channel::getNameOperator()
{
    return (_nameOperator);
}


