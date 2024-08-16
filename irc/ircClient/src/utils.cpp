/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:48:08 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/01 17:52:09 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

std::string splitBuffer(std::string buffer)
{
    // char sep = '\x1F';
    size_t sepa = buffer.find('\x1F');
    if (sepa != std::string::npos)
    {
        std::string login = buffer.substr(0, sepa);
        return (login);
    }
    else
    {
        std::cout << "error on split" << std::endl;
        return (buffer);
    }
}

std::string splitBuffer2(std::string buffer)
{
    // char sep = '\x1F';
    size_t sepa = buffer.find('\x1F');
    if (sepa != std::string::npos)
    {
        std::string msg = buffer.substr(sepa + 1);
        return (msg);
    }
    else
    {
        std::cout << "error on split" << std::endl;
        return (buffer);
    }
}