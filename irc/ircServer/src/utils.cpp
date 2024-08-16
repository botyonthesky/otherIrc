/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:50:13 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/01 17:49:35 by botyonthesk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return ("");
    }
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

std::string reduce_spaces(const std::string &str) 
{
    std::string result;
    bool in_space = false;
    for (size_t i = 0; i < str.length(); ++i) 
    {
        if (std::isspace(str[i])) 
        {
            if (!in_space)
            {
                result += ' ';
                in_space = true;
            }
        }
        else 
        {
            result += str[i];
            in_space = false;
        }
    }
    return (result);
}

std::string trim_and_reduce_spaces(const std::string& str)
{
    std::string result;
    result = trim(str);
    result = reduce_spaces(result);
    return (result);
}

