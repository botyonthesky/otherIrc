/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:03:04 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/05 15:17:3 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

user::user(server& srv, int clientFd, std::vector<std::string> command): _server(srv), _clientFd(clientFd), _inChannel(false)
{
    addInfo(command);
    _server.setLogin(_username);
    _server.nicknameClient.push_back(_nickname);
}

user::~user()
{
    _server.delUserList(this);
    std::cout << this->_username << " has quit the server" << std::endl;
}

void    user::addInfo(std::vector<std::string> command)
{
   for(size_t i = 0; i < command.size(); i++)
   {
        if (command[i].find("NICK") == 0)
            processNick(command, i);
        if (command[i].find("USER") == 0)
            processUser(command, i);
   }
   _currChannel = "No channel";
}

void    user::processUser(std::vector<std::string> command, size_t& i)
{
    std::vector<std::string> info = _server.parsingIntoVector(command[i]);
    _username = info[1];
    _hostname = info[3];
    _realName = extractRealName(info);
}
std::string    user::extractRealName(std::vector<std::string> info)
{
    std::string real = info[4];
    size_t pos = real.find(':');
    if (pos != std::string::npos)
    {
        real.erase(0, 1);
        if (info.size() > 5)
        {
            real += " " + info[5];
            return (real);
        }
    }
    return (real);
}
void    user::processNick(std::vector<std::string> command, size_t& i)
{
    size_t pos = command[i].find(" ");
    if (pos != std::string::npos)
        _nickname = command[i].substr(pos + 1);
    if (_server.isValidNickname(_nickname) == false)
        _nickname = _nickname + toStr(_clientFd);
    i++;
}

void    user::help()
{
    if (this->_nickname[0] == '@')
    {
        std::string msg = "\nSpecific operator commands ->\n"
                        "KICK [nick name] [channel]         eject an user of the channel\n"
                        "INVITE [nick name] [channel]       invite an user to the channel\n"
                        "TOPIC [channel] [subject]          define subject as the topic of the channel\r\n";
        _server.sendMessage(this, NOCODE, msg);
    }
    std::string msg = "\nBasic commands ->\n"
                        "/INFO                              display your information\n"
                        "/NICK [nick name]                  change your nickname\n"
                        "/USER [user name]                  change your login\n"
                        "/JOIN [channel]                    join channel\n"
                        "/PART                              leave current channel\n"
                        "/QUIT                              quit IRC\n"
                        "/WHO                               list of users in current channel\n"
                        "/MSG [nick name] [msg]             submit msg to login\n"
                        "/LIST                              list of channel\n"
                        "[msg]                              send msg to current channel\r\n";

    _server.sendMessage(this, NOCODE, msg);
}

void    user::info()
{
    std::string msg = "\nYour username is : " + _username +
    "\nYour nickname is : " + _nickname +
    "\nYour hostname is : " + _hostname +
    "\nYour realname is : " + _realName;
    std::string channel;
    if (!_inChannel)
        channel = "\nYou re not in any channel right now !\r\n";
    else
        channel = "\nYou re in the channel : " + _currChannel + "\r\n";
    _server.sendMessage(this, NOCODE, msg + channel);
    user::help();
}

void    user::nick()
{
    if (_server.getCommand().size() != 2)
    {
        _server.sendMessage(this, ERR_UNKNOWNCOMMAND, ":Unknow command\r\n");
        return ;
    }
    try
    {
        if (!isValidNickname(_server.getCommand()[1]))
        {
            _server.sendMessage(this, ERR_ERRONEUSNICKNAME, ("Erroneous " + _server.getCommand()[1] + "\r\n"));
            throw NotValidNickName();
        }
        std::string oldNick = _nickname;
        std::string newNick = _server.getCommand()[1];
        _server.updateNicknameList(oldNick, newNick);
        std::string msg = "Your nickname was : " + oldNick + " its now : " + newNick + "\r\n";
        if (_nickname[0] == '@')
            _nickname = "@" + newNick;
        else
            _nickname = newNick;
        _server.sendMessage(this, RPL_MYINFO, msg);
        std::cout << oldNick << " has changed his nickname to : " << _nickname << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void   user::userName()
{
    if (_server.getCommand().size() != 5)
    {
        _server.sendMessage(this, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
        return ;
    }
    try
    {
        if (!isValidUsername(_server.getCommand()[1]))
        {
            _server.sendMessage(this, ERR_ERRONEUSNICKNAME, ("Erroneous " + _server.getCommand()[1] + "\r\n"));
            throw NotValidUserName();
        }
        std::string newName = _server.getCommand()[1];
        std::string oldName = _username;
        _server.updateLoginList(oldName, newName);
        _username = newName;
        _hostname = _server.getCommand()[2];
        _realName = _server.getCommand()[4];
        std::string msg = "You username was : " + oldName + ", it's now : " + newName + "\r\n";
        _server.sendMessage(this, RPL_MYINFO, msg);
        std::cout << oldName << " has changed his username to : " << newName << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void    user::join()
{
    if (_server.getCommand().size() != 2)
    {
        _server.sendMessage(this, ERR_UNKNOWNCOMMAND, ":Unknow command\r\n");
        return ;
    }
    std::string name = _server.getCommand()[1];
    try
    {
        if (!isValidChannelName(name))
        {
            _server.sendMessage(this, ERR_NOSUCHCHANNEL, (name + " :Not such Channel\r\n"));
            throw NotValidChannelName();
        }
        if (_currChannel != "No channel")
            leave();
        if (checkChannel() == -1)
            createNewChannel(name);
        else
            joinChannel(name);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void    user::leave()
{
   if (_currChannel == "No channel")
    {
        std::string msg = "Nochannel : You're not on that channel\r\n";
        _server.sendMessage(this, ERR_NOTONCHANNEL, msg);
    }
    else
    {
        channel * curr = getChannelByName(_currChannel);
        int idx = curr->getIdxUserByNickname(this->_nickname);
        curr->delUserN(idx);
        if (_nickname[0] == '@')
            setNickname(_nickname.erase(0, 1));
        _inChannel = false;
        std::string msg = "PART " + _currChannel + " :Leaving the channel\r\n";
        for (int i = 1; i <= curr->getNbUser(); i++)
            _server.SendSpeMsg(this, curr->getUserN(i), msg);
        _server.checkChannel(_currChannel);
        _server.SendSpeMsg(this, this, ("PART " + _currChannel));
        for (int i = 1; i <= curr->getNbUser(); i++)
            _server.SendSpeMsg(this, curr->getUserN(i), "PART " + _currChannel);
        _currChannel = "No channel";
        _server.infoClient(_clientFd);
    }
}

void    user::who()
{
    std::string msg;
    if (!_inChannel)
        _server.sendMessage(this, ERR_NOSUCKNICK, ":No such channel\r\n");
    else
    {
        channel *curr = getChannelByName(_currChannel);
        if (curr == NULL)
        {
            std::cerr << "Error on /who" << std::endl;
     
            return ;
        }
        for (int i = 1; i <= curr->getNbUser(); i++)
        {
            std::string nick = curr->getUserN(i)->getNick();
            std::string user = curr->getUserN(i)->getUsername();
            std::string host = curr->getUserN(i)->getHostname();
            std::string real = curr->getUserN(i)->getReal();
            msg = _currChannel + " " + user + " " + host + " " + _server.name
            + " " + nick + " :" + real;  
            _server.sendMessage(this, RPL_WHOREPLY, msg);
        }
        _server.sendMessage(this, RPL_ENDOFWHO, (_currChannel + " :End of WHO list\r\n"));
    }
}
int     user::privOrChan(std::string input)
{
    if (input[0] == '&' || input[0] == '#')
        return (2);
    else
        return (1);
}

void    user::msg()
{
    int choice = privOrChan(_server.getCommand()[1]);
    if (choice == 1)
    {
        std::string nickname = _server.getCommand()[1];
        std::string message;
        for(size_t i = 2; i < _server.getCommand().size(); i++)
        {
            message += _server.getCommand()[i];
            message += " ";
        }
        message.erase(0, 1);
        if (!checkNicknameList(nickname))
            _server.sendMessage(this, ERR_NOSUCKNICK, "No such nick\r\n");
        else
        {
            user * toSend = _server.getUserByNickname(nickname);
            std::string msg = "PRIVMSG " + nickname + " :" + message; 
            _server.SendSpeMsg(this, toSend, msg);
        }
    }
    else
    {
        std::string name = _server.getCommand()[1];
        std::string message;
        for(size_t i = 2; i < _server.getCommand().size(); i++)
        {
            message += _server.getCommand()[i];
            message += " ";
        }
        if (!this->_inChannel)
            _server.sendMessage(this, ERR_CANNOTSENDTOCHAN, (name + " :Cannot send to channel"));
        else
        {
            channel * curr = getChannelByName(name);
            if (curr == NULL)
                return ;
            if (_currChannel != name)
            {
                _server.sendMessage(this, ERR_NOTONCHANNEL, " :You're not on that channel");
                return ;
            }
            for (int i = 1; i <= curr->getNbUser(); i++)
                _server.SendSpeMsg(this, curr->getUserN(i), ("PRIVMSG " + name + " " + message));
        }
    }
}

void    user::list()
{
    std::string msg;
    if (_server.getNbChannel() == 0)
        _server.sendMessage(this, RPL_LISTEND, " :End of LIST\r\n");
    else
    {      
        for (int i = 1; i <= _server.getNbChannel(); i++)
        {
            std::string name = _server.channelId[i]->getName();
            std::string topic = _server.channelId[i]->topic;
            std::string nbs = toStr(_server.channelId[i]->getNbUser());
            msg = name + " " + nbs + " :" + topic + "\r\n";
            _server.sendMessage(this, RPL_LIST, msg);
        } 
        _server.sendMessage(this, RPL_LISTEND, " :End of LIST\r\n");
    }
   }

void    user::kick()
{
    if (!checkKickInfo())
        return ;
    else
    {
        std::string nameK = _server.getCommand()[2];
        user * toKick = findUserChannelByName(nameK);
        if (toKick == NULL)
        {
            std::cout << "User is null" << std::endl;
            return ;
        }
        channel * curr = getChannelByName(_currChannel);
        int idx = curr->getIdxUserByNickname(nameK);
        if (idx == -1)
        {
            std::cout << "Wrong idx" << std::endl;
            return ;
        }
        curr->delUserN(idx);
        toKick->_currChannel = "No channel";
        toKick->_inChannel = false;
        std::string msg = "KICK " + _currChannel + " " + nameK + "\r\n";
        for (int i = 1; i <= curr->getNbUser(); i++)
            _server.SendSpeMsg(this, curr->getUserN(i), msg);
        _server.checkChannel(_currChannel);
        _server.infoClient(toKick->getClientFd());
    }
    return ;
}

void    user::invite()
{   
    if (!inviteCommandCheck(_server.getCommand()[2]))
        return ;
    else
    {
        std::string nickTo = _server.getCommand()[1];
        std::string channelTo = _server.getCommand()[2];
        user * toSend = _server.getUserByNickname(nickTo);
        if (toSend == NULL)
            std::cout << "error on invite, tosend is null" << std::endl;
        else
        {
            std::string msg = "INVITE " + nickTo + " :" + channelTo + "\r\n";
            _server.sendMessage(this, RPL_INVITING, (nickTo + " " + channelTo + "\r\n"));
            _server.SendSpeMsg(this, toSend, msg);
        }
    }
}

void    user::defTopic()
{
    if (!topicCommandCheck(_server.getCommand()[1]))
        return ;
    else if (_server.getCommand().size() >= 3)
    {
        std::string channelName = _server.getCommand()[1];
        std::string newTopic = mergeCommand(_server.getCommand());
        channel * curr = getChannelByName(channelName);
        curr->topic = newTopic;
        curr->timestamp();
        for (int i = 1; i <= curr->getNbUser(); i++)
        {
            _server.sendMessage(curr->getUserN(i), RPL_TOPIC, (channelName + " :" + newTopic));
            _server.sendMessage(curr->getUserN(i), RPL_TOPICWHOTIME, (channelName + " " + _nickname + " " + curr->createTime));
        }
    }
}
void    user::ping()
{
    _server.sendMessage(this, NOCODE, ("PONG " + _server.name));
}
void    user::mode()
{
//    if (_server.getCommand().size() != 3)
//    {
//         std::cout << "mode" << std::endl;
//    } 
}

std::string     user::mergeCommand(std::vector<std::string> command)
{
    std::string result;
    size_t i = 2;
    while(i <= command.size() - 2)
    {
        result += command[i] + " ";
        i++;
    }
    result += command[i];
    result.erase(0, 1);
    return (result);
}

void    user::topic()
{
    std::cout << "topic" << std::endl;
}

bool    user::topicCommandCheck(std::string channel)
{
    if (_nickname[0] != '@')
    {
        _server.sendMessage(this, ERR_CHANOPRIVSNEEDED, " :You're not channel operator\r\n");
        return (false);
    }
    if (checkChannel2(channel) == -1)
    {
        _server.sendMessage(this, ERR_NOSUCHCHANNEL, " :No such channel");
        return (false);
    }
    return (true);
}

void    user::createNewChannel(std::string name)
{
    channel *newChannel = new channel(this, name);
    _inChannel = true;
    _currChannel = name;
    _server.setNbChannel(1);
    newChannel->setIdx(_server.getNbChannel());
    _server.channelId[newChannel->getIdx()] = newChannel;
    _server.sendMessage(this, RPL_TOPIC, (name + " " + newChannel->topic));
    std::string time = newChannel->createTime;
    // _server.groupMsg(this, RPL_TOPICWHOTIME, newChannel, "test");
    _server.sendMessage(this, RPL_TOPICWHOTIME, (name + " " + this->getNick() + " " + time));
    _server.sendMessage(this, RPL_NAMREPLY, ("= " + name + " :@" + this->getNick()));
    _server.sendMessage(this, RPL_ENDOFNAMES, (name + " :End of /NAMES list"));
    registerChannel(name, newChannel);
    if(_nickname[0] != '@')
        _nickname =  "@" + _nickname;
    _server.infoClient(_clientFd);
}

void    user::joinChannel(std::string name)
{
    _inChannel = true;
    _currChannel = _server.getCommand()[1];
    _server.channelId[checkChannel()]->setNbUser(1);
    registerChannel(name, _server.channelId[checkChannel()]);
    channel * curr = getChannelByName(name);
    int idx = _server.channelId[checkChannel()]->getNbUser();
    curr->setUserN(this, idx);
    if (curr->getNameOperator() == _nickname)
        _nickname = "@" + _nickname;
    _server.sendMessage(this, RPL_TOPIC, (name + " " + curr->topic + "\r\n"));
    std::string op = curr->getNameOperator();
    _server.groupMsg(this, RPL_TOPICWHOTIME, curr, "test");
    _server.sendMessage(this, RPL_TOPICWHOTIME, (name + " " + op + " " + curr->createTime + "\r\n"));
    _server.sendMessage(this, RPL_NAMREPLY, ("= " + name + " :" + curr->getNameOperator() + this->getNick()));
    for (int i = 1; i <= curr->getNbUser(); i++)
        _server.SendSpeMsg(this, curr->getUserN(i), ("JOIN :" + name));
    _server.infoClient(_clientFd);

}

int    user::checkChannel2(std::string name)
{
    for (int i = 1; i <= _server.getNbChannel(); i++)
    {
        if (_server.channelId[i]->getName() == name)
            return (_server.channelId[i]->getIdx());
    }
    return (-1);
}
int    user::checkChannel()
{
    for (int i = 1; i <= _server.getNbChannel(); i++)
    {
        if (_server.getCommand()[1] == _server.channelId[i]->getName())
            return (_server.channelId[i]->getIdx());
    }
    return (-1);
}
bool    user::checkUser()
{
    if (_server.getCommand().size() != 3)
    {
        _server.sendMessage(this, ERR_UNKNOWNCOMMAND, " :Unknow command\r\n");
        return (false);
    }
    if (_inChannel == false)
    {
        _server.sendMessage(this, ERR_NOSUCHCHANNEL, (_server.getCommand()[2] + " :No such channel\r\n"));
        return (false);
    }
    if (_currChannel != _server.getCommand()[2])
    {
        _server.sendMessage(this, ERR_NOTONCHANNEL, (_server.getCommand()[2] + " :You're not on that channel\r\n"));
        return (false);
    }
    if (_nickname[0] != '@')
    {
        _server.sendMessage(this, ERR_CHANOPRIVSNEEDED, (_server.getCommand()[2] + " You're not channel operator\r\n"));
        return (false);
    }
    return (true);
}
bool    user::checkUserChannelList(std::string nickname)
{
    channel * curr = getChannelByName(_currChannel);
    for (int i = 1; i <= curr->getNbUser(); i++)
    {
        if (curr->getUserN(i)->getNick() == nickname)
            return (true);
    }
    return (false);
}


bool    user::checkNicknameList(std::string nickname)
{
    for (int i = 1; i <= _server.getNbClient(); i++)
    {
        if (_server.getUserN(i)->getNick() == nickname)
            return (true);
    }
    return (false);
}
bool    user::checkKickInfo()
{
    std::string channel = _server.getCommand()[3].erase(0,1);
    std::string toKick = _server.getCommand()[2];
    if (_server.getCommand().size() != 4)
    {
        _server.sendMessage(this, ERR_UNKNOWNCOMMAND, " :Unknow command\r\n");
        return (false);
    }
    if (_inChannel == false)
    {
        _server.sendMessage(this, ERR_NOSUCHCHANNEL, (_server.getCommand()[2] + " :No such channel\r\n"));
        return (false);
    }
    if (_currChannel != channel)
    {
        _server.sendMessage(this, ERR_NOTONCHANNEL, (_server.getCommand()[2] + " :You're not on that channel\r\n"));
        return (false);
    }
    if (_nickname[0] != '@')
    {
        _server.sendMessage(this, ERR_CHANOPRIVSNEEDED, (_server.getCommand()[2] + " You're not channel operator\r\n"));
        return (false);
    }
    if (!checkUserChannelList(toKick))
    {
        _server.sendMessage(this, ERR_NOSUCKNICK, (_server.getCommand()[1] + " :no such nick\r\n"));
        return (false);
    }
    return (true);
}

void    user::registerChannel(std::string name, channel * channel)
{
    for (int i = 1; i <= _server.getNbChannel(); i++)
    {
        if (_server.channelId[i]->getName() == name)
            channelUser[i] = channel;
    }
}   

user*  user::findUserChannelByName(std::string name)
{
    channel * curr = getChannelByName(_currChannel);
    if (curr == NULL)
        return (NULL);
    for (int i = 1; i <= curr->getNbUser(); i++)
    {
        if (curr->getUserN(i)->getNick() == name)
            return (curr->getUserN(i));
    }
    return (NULL);
}

bool    user::inviteCommandCheck(std::string channel)
{
    std::string toInvit = _server.getCommand()[1];
    if (_server.getCommand().size() != 3)
    {
        _server.sendMessage(this, ERR_UNKNOWNCOMMAND, " :Unknow command\r\n");
        return (false);
    }
    if (_nickname[0] != '@')
    {
        _server.sendMessage(this, ERR_CHANOPRIVSNEEDED, (channel + " :You're not channel operator\r\n"));
        return (false);
    }
    if(!checkNicknameList(toInvit))
    {
        _server.sendMessage(this, ERR_NOSUCKNICK, (toInvit + " : No such nick\r\n"));
        return (false);
    }
    if (checkUserChannelList(toInvit))
    {
        _server.sendMessage(this, ERR_USERONCHANNEL, (toInvit + " " + channel + " :is already on channel"));
        return (false);
    }
    return (true);
}

bool    user::isValidChannelName(std::string name)
{
    if (name[0] == '&' || name[0] == '#')
    {
        for (size_t i = 0; i < name.size(); i++)
            if(!isprint(name[i]))
                return (false);
        return (true);
    }
    return (false);
}

bool    user::isValidUsername(std::string username)
{
    if (!isalpha(username[0]))
        return (false);
    return (true);
}

bool    user::isValidNickname(std::string nickname)
{
    for (std::vector<std::string>::iterator it = _server.nicknameClient.begin(); it != _server.nicknameClient.end(); it++)
    {
        if (*it == nickname)
            return (false);
    }
    if (!isalpha(nickname[0]) || nickname[0] == '@')
        return (false);
    return (true);
}

bool        user::isValidChannel(std::string name)
{
    for (int i = 1; i <= _server.getNbChannel(); i++)
    {
        if (_server.channelId[i]->getName() == name)
            return (true);
    }
    return (false);

}

channel*     user::getChannelByName(std::string name)
{
    if (isValidChannel(name))
    {
        for (int i = 1; i <= _server.getNbChannel(); i++)
        {
            if (_server.channelId[i]->getName() == name)
                return (channelUser[i]);
        }
    }
    _server.sendMessage(this, ERR_NOSUCHCHANNEL, (name + " :No such channel"));
    return (NULL);
}
int             user::getClientFd(void)
{
    return (_clientFd);    
}
int            user::getIdx(void)
{
    return (_idx);
}

void        user::setIdx(int idx)
{
    _idx = idx;
}

void        user::decrementIdx()
{
    _idx--;
}

std::string     user::getUsername(void)
{
    return (_username);
}

std::string     user::getNick(void)
{
    return (_nickname);
}
std::string     user::getCurrChannel(void)
{
    return (_currChannel);
}
bool            user::getInChannel()
{
    return (_inChannel);
}

channel*    user::getChannelByIdx(int idx)
{
    return (channelUser[idx]);
}

std::string    user::getHostname()
{
    return (_hostname);
}

std::string     user::getReal()
{
    return (_realName);
}

void    user::setNickname(std::string nickname)
{
    _nickname = nickname;
}
const char*     user::NotValidUserName::what() const throw()
{
    return ("The username is not valid !");
}

const char*     user::NotValidNickName::what() const throw()
{
    return ("The nickname is not valid !");
}

const char* user::NotValidChannelName::what() const throw()
{
    return ("The channel name is not valid\nUsage : ('#' | '&') <chstring>");
}

