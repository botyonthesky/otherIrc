/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:24:56 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/07/23 17:28:55 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client.hpp"

int main(int argc, char **argv)
{
    if (argc != 1)
    {
        std::cout << "Usage : ./clientIrc" << std::endl;
        return (1);
    }
    else
    {
        try
        {
            client clientIrc;
            clientIrc.run();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
    
    (void)argv;
    return (0);
}
