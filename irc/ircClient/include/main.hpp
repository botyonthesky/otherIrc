/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:07:04 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/19 09:42:21 by tmaillar         ###   ########.fr       */
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
#include <cstdlib>

#include "utils.hpp"
#include "client.hpp"

#define MAXCLIENT 10
#define MAXCHANNEL 3
#define PORT 4343


#endif
