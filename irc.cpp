#include "irc.hpp"
int main(int argc, char **argv)
{

	//basic parsing - needs protection
	(void)argc;
	int listenPort = atoi(argv[1]);

	// AF_INET is IP v4 internet protocols
	// SOCK_STREAM reliable, two-way connection-based byte streams
	//Create a socket for communication
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		exitWithMsg("socket(): error");
	//Assign a name/address to the socket file descriptor
	struct sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(listenPort);
	if (bind(socketFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
		exitWithMsg("bind(): error");

	//Listen to any incoming connections
	if (listen(socketFd, MAX_CONNECTIONS) == -1)
		exitWithMsg("listen(): error");

	//Manage multiple file descriptors without blocking using poll
	struct pollfd fdSet[1];
	fdSet->fd = socketFd;
	fdSet->events = POLLIN;
	
	int retPoll = poll(fdSet, 1, TIMEOUT);
	if (retPoll == -1)
		exitWithMsg("poll(): error");
	else if(retPoll == 0)
	{
		//timeout
		;
	}
	else
	{
		int client_sock_fd = accept(socketFd, NULL, NULL);
		if (client_sock_fd == -1)
			exitWithMsg("accept(): error");
		
		char buffer[1024];
		int bytes = recv(client_sock_fd, buffer, sizeof(buffer), 0);
		std::cout << "Number of bytes recieved: " << bytes << std::endl;
		std::cout << buffer << std::endl;
	}

	//Accept a client connection
	// struct sockaddr_in clientAddr;
	// socklen_t clientAddrLen =  sizeof(clientAddr);
	// int clientSockFd = accept(socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	// if (clientSockFd == -1)
	// {
	// 	exit (1);
	// }
}