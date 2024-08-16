/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:07:04 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/16 18:14:20 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <poll.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <string>
#include <netdb.h>
#include <cstdio>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <climits>
#include <vector>
#include <cstring> 
#include <sys/time.h>
#include <ctime>

#include "server.hpp"
#include "user.hpp"
#include "channel.hpp"
#include "utils.hpp"

#define MAXCLIENT 10
#define MAXCHANNEL 3
#define PORT 4242

#define RPL_WELCOME 001
#define RPL_YOURHOST 002
#define RPL_CREATED 003
#define RPL_MYINFO 004

#define NOCODE -42

#define RPL_ENDOFWHO 315

#define RPL_LIST 321
#define RPL_LISTEND 323

#define RPL_NOTOPIC 331
#define RPL_TOPIC 332
#define RPL_TOPICWHOTIME 333

#define RPL_INVITING 341

#define RPL_NAMREPLY 353

#define RPL_WHOREPLY 352

#define RPL_ENDOFNAMES 366

#define ERR_NOSUCKNICK 401
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_NOSUCHCHANNEL 403

#define ERR_UNKNOWNCOMMAND 421

#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTONCHANNEL 442

#define ERR_CHANOPRIVSNEEDED 482

#endif 