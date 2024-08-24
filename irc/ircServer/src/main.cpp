/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:45:13 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/24 18:53:57 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Invalid argument. Usage : ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    server ircserv(argv[1], argv[2]);
    ircserv.run();       
    
    return (0);
}