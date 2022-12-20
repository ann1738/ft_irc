#include "irc.hpp"
#include "initialParse.hpp"
#include <cstring>
#include <unistd.h>
#include <vector>

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
	int opt = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//Assign a name/address to the socket file descriptor
	struct sockaddr_in sockAddr, clientAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(IP);
	sockAddr.sin_port = htons(init.getPort());
	if (bind(socketFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
		exitWithMsg("bind(): error");

	//Listen to any incoming connections
	if (listen(socketFd, MAX_CONNECTIONS) == -1)
		exitWithMsg("listen(): error");

	char buffer[MAX_MSG_LENGTH];

	int fd_num = 0;
	fd_set readySockets;
	std::vector<struct pollfd>	clientSockets;

	clientSockets.resize(++fd_num);
	clientSockets[0].fd = socketFd;
	clientSockets[0].events = POLLIN;

	std::cout << "entering main loop" << std::endl;
	
	while (true)
	{
		// readySockets = clientSockets;
		int pollReturn = poll(clientSockets, fd_num, -1)
		std::cout << "AFTER SELECT()" << std::endl;
		if (pollReturn == -1)
		{
			std::cerr << "poll()" << std::endl;
			exit(1);
		}
		for(int i = 0; i <= fd_num; i++)
		{
			if (clientSockets[i].revents & POLLIN)
			{
				std::cout << "Found an fd" << std::endl;
				if (i == socketFd)
				{
					std::cout << "New connection!!!" << std::endl;
					//new connection incomming
					int clientSocketFd = accept(socketFd, NULL, NULL);
					(void)clientAddr;
					// int clientSocketFd = accept(socketFd, (struct sockaddr *)&clientAddr, (socklen_t *)sizeof(&clientAddr));
					if (clientSocketFd == -1)
					{
						std::cerr << "accept(): error" << std::endl;
						// FD_CLR(clientSocketFd, &clientSockets); //remove fd from connections
					}
					else
					{
						fcntl(clientSocketFd, F_SETFL, O_NONBLOCK);
						/* add client socket */
						struct pollfd temp;
						temp.fd = clientSocketFd;
						temp.events = POLLIN;
						clientSocketFd.push_back(temp);
						fd_num++;
						// FD_SET(clientSocketFd, &clientSockets);
						// if (clientSocketFd > maxFd)
							// maxFd = clientSocketFd;
						// maxFd = clientSocketFd > maxFd ? clientSocketFd : maxFd; //update max fd
						std::cout << "New client with fd: " << clientSocketFd << std::endl;
					}
				}
				else
				{
					std::cout << "Old connection ;)" << std::endl;
					//handle new connection
					int readBytes = recv(i, buffer, sizeof(buffer), 0);
					if (readBytes == -1)
					{
						std::cerr << "recv()" << std::endl;
						FD_CLR(i, &clientSockets); //remove fd from connections
						// exit(1);
					}
					else if (readBytes == 0) //connection closed
					{
						FD_CLR(i, &clientSockets); //remove fd from connections
						std::cout << "Connection closed with fd: " << i << std::endl;
						// close(i);
					}
					else
					{
						std::cout << buffer << std::endl;
					}
				}
			}
		}
	}

// 	int maxFd = socketFd;

// 	fd_set readySockets;
// 	fd_set clientSockets;

// 	FD_ZERO(&clientSockets);
// 	FD_ZERO(&readySockets);
// 	FD_SET(socketFd, &clientSockets);

// 	std::cout << "entering main loop" << std::endl;
	
// 	while (true)
// 	{
// 		readySockets = clientSockets;
// 		int sel = select(maxFd + 1, &readySockets, NULL, NULL, NULL); //add timeout
// 		std::cout << "AFTER SELECT()" << std::endl;
// 		if (sel == -1)
// 		{
// 			std::cerr << "select()" << std::endl;
// 			exit(1);
// 		}
// 		for(int i = 0; i <= maxFd; i++)
// 		{
// 			if (FD_ISSET(i, &readySockets))
// 			{
// 				std::cout << "Found an fd" << std::endl;
// 				if (i == socketFd)
// 				{
// 					std::cout << "New connection!!!" << std::endl;
// 					//new connection incomming
// 					int clientSocketFd = accept(socketFd, NULL, NULL);
// 					(void)clientAddr;
// 					// int clientSocketFd = accept(socketFd, (struct sockaddr *)&clientAddr, (socklen_t *)sizeof(&clientAddr));
// 					if (clientSocketFd == -1)
// 					{
// 						std::cerr << "accept(): error" << std::endl;
// 						FD_CLR(clientSocketFd, &clientSockets); //remove fd from connections
// 					}
// 					else
// 					{
// 						fcntl(clientSocketFd, F_SETFL, O_NONBLOCK);
// 						FD_SET(clientSocketFd, &clientSockets);
// 						if (clientSocketFd > maxFd)
// 							maxFd = clientSocketFd;
// 						// maxFd = clientSocketFd > maxFd ? clientSocketFd : maxFd; //update max fd
// 						std::cout << "New client with fd: " << clientSocketFd << std::endl;
// 					}
// 				}
// 				else
// 				{
// 					std::cout << "Old connection ;)" << std::endl;
// 					//handle new connection
// 					int readBytes = recv(i, buffer, sizeof(buffer), 0);
// 					if (readBytes == -1)
// 					{
// 						std::cerr << "recv()" << std::endl;
// 						FD_CLR(i, &clientSockets); //remove fd from connections
// 						// exit(1);
// 					}
// 					else if (readBytes == 0) //connection closed
// 					{
// 						FD_CLR(i, &clientSockets); //remove fd from connections
// 						std::cout << "Connection closed with fd: " << i << std::endl;
// 						// close(i);
// 					}
// 					else
// 					{
// 						std::cout << buffer << std::endl;
// 					}
// 				}
// 			}
// 		}
// 	}



}