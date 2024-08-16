/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:45:13 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/10 12:12:11 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    server ircserv;
    ircserv.run();       
    
    return (0);
}