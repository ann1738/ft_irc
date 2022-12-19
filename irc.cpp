#include "irc.hpp"
#include "initialParse.hpp"
#include <cstring>

int main(int argc, char **argv)
{
	//basic parsing - needs protection
	// (void)argc;
	// int listenPort = atoi(argv[1]);
	initialParse init;
	init.parse(argc, argv);

	// AF_INET is IP v4 internet protocols
	// SOCK_STREAM reliable, two-way connection-based byte streams


	//Create a socket for communication
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		exitWithMsg("socket(): error");

	fcntl(socketFd, F_SETFL, O_NONBLOCK);

	//look into setsockopt()

	//Assign a name/address to the socket file descriptor
	struct sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(IP);
	sockAddr.sin_port = htons(init.getPort());
	if (bind(socketFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
		exitWithMsg("bind(): error");

	//Listen to any incoming connections
	if (listen(socketFd, MAX_CONNECTIONS) == -1)
		exitWithMsg("listen(): error");


	// int	activeClients = 1;
	// int	clientFdArray[MAX_CLIENTS];

	// memset(&clientFdArray, 0, MAX_CLIENTS);

	accept(socketFd, NULL, NULL);
	return 0;	
	char buffer[MAX_MSG_LENGTH];

	fd_set readySockets;
	fd_set clientSockets;

	FD_ZERO(&clientSockets);
	FD_SET(socketFd, &clientSockets);

	while (true)
	{
		readySockets = clientSockets;
		int sel = select(FD_SETSIZE, &readySockets, NULL, NULL, NULL); //add timeout
		if (sel == -1)
		{
			std::cerr << "select()" << std::endl;
			exit(1);
		}
		for(int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &readySockets))
			{
				if (i == socketFd)
				{
					//new connection incomming
					int clientSocketFd = accept(socketFd, NULL, NULL);
					fcntl(clientSocketFd, F_SETFL, O_NONBLOCK);
					FD_SET(clientSocketFd, &clientSockets);
				}
				else
				{
					//handle new connection
					if (recv(i, buffer, sizeof(buffer), 0) == -1)
					{
						std::cerr << "recv()" << std::endl;
						exit(1);
					}
					std::cout << buffer << std::endl;
					FD_CLR(i, &clientSockets); //remove fd from connections
				}
			}
		}
	}



	//Manage multiple file descriptors without blocking using poll
	// struct pollfd fdSet[MAX_CLIENTS];
	// fdSet->fd = socketFd;
	// fdSet->events = POLLIN;

	// for (int i = 1; i < MAX_CLIENTS; i++)
	// {
	// 	fdSet[i].fd = 0;
	// 	fdSet[i].events = POLLIN;
	// }
	// char buffer[MAX_MSG_LENGTH];

	// while (true)
	// {
	// 	// std::cout << "Before poll()" << std::endl;
	// 	int retPoll = poll(fdSet, activeClients, TIMEOUT);
	// 	// std::cout << "After poll()" << std::endl;
	// 	if (retPoll == -1)
	// 	{
	// 		exitWithMsg("poll(): error");
	// 		exit(1);
	// 	}
	// 	else if(retPoll == 0)
	// 	{
	// 		std::cout << "poll(): timeout" << std::endl;
	// 		exit(1);
	// 	}
	// 	else
	// 	{
	// 		// std::cout << "Did it go to the proper place?" << std::endl;
	// 		if (fdSet[0].revents & POLLIN) //Data is available
	// 		{
	// 			std::cout << "Data is available" << std::endl;
	// 			int client_sock_fd = accept(socketFd, NULL, NULL);
	// 			if (client_sock_fd == -1)
	// 				exitWithMsg("accept(): error");
	// 			for (int i = 1; i < MAX_CLIENTS; i++)
	// 			{
	// 				if (fdSet[i].fd == client_sock_fd)
	// 					break;
	// 				if (fdSet[i].fd == 0)
	// 				{
	// 					std::cout << "New socket!!" << std::endl;
	// 					fcntl(client_sock_fd, F_SETFL, O_NONBLOCK);
	// 					fdSet[i].fd = client_sock_fd;
	// 					activeClients++;
	// 					break;
	// 				}
	// 			}
	// 			recv(client_sock_fd, buffer, sizeof(buffer), 0);
	// 			std::cout << buffer;
	// 			memset(buffer, 0, strlen(buffer));
	// 		}
	// 	}
	// }

}