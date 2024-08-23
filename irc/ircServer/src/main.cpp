/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:45:13 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/23 09:06:51 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (argc != 3)
    {
        std::cout << "Invalid argument. Usage : ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    server ircserv(argv[1], argv[2]);
    ircserv.run();       
    
    return (0);
}