#ifndef IRC_HPP
#define IRC_HPP

#define MAX_CONNECTIONS 10
#define TIMEOUT 1000000000
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

void	exitWithMsg(std::string msg);

#endif