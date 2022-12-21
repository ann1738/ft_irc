#ifndef IRC_HPP
#define IRC_HPP



#define IP "127.0.0.1"
#define MAX_CONNECTIONS 10
#define MAX_CLIENTS 1000
#define TIMEOUT 100000
#define MAX_MSG_LENGTH 512

#include <iostream>
/* atoi() */
#include <cstdlib>
#include <string>

/*socket()*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>
//this is for conenct()
#include <sys/types.h>
#include <fcntl.h>

void	exitWithMsg(std::string msg);

#endif