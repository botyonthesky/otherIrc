/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:21:24 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/08/19 09:42:16 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>


#define PORT 4343

class client
{
    private:
        
        struct sockaddr_in  _sa;
        int                 _socketFd;
        int                 _status;
        int                 _bytesRead;
        std::string         _msg;
        int                 _msgLen;
        int                 _bytesSent;

        
    public:
    
        client();
        ~client();
        
        void    setMessage(std::string msg);

        void    run(void);
        void    initClient(void);
        void    initSocket(void);
        void    connectServer(void);
        void    sendMessage(void);
        void    receivedMessage(void);
        void    handleMessage(void);

        class socketFdError : public std::exception
        {
            virtual const char* what() const throw();  
        };
        
        class recvError : public std::exception
        {
            virtual const char* what() const throw();  
        };

        class sendError : public std::exception
        {
            virtual const char* what() const throw();
        };
        
        class connectError : public std::exception
        {
            virtual const char* what() const throw();
        };

        class exitSocket : public std::exception
        {
            public:
                virtual const char* what() const throw();   
        };
};



#endif