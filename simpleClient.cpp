#include "irc.hpp"
     #include <sys/types.h>
     #include <sys/socket.h>
	 #include <iostream>

#define IP "127.0.0.1"
#define PORT 6667

int main()
{
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	serverAddr.sin_port = htons(PORT);
	
	int ret = connect(socketFd, (sockaddr *)&serverAddr, sizeof(serverAddr));

	const char * msg = "Helllooo from the clieeeeent!";
	int err = send(socketFd, msg, sizeof(msg), 0);
	if (err == -1)
	{
		// std::cout << "Error sending msg" << stdendl;
		std::cout << "Error";
	}

}