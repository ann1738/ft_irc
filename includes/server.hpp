#ifndef SERVER_HPP
# define SERVER_HPP

/*socket()*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>
#include <fcntl.h>
// #include <exception>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

#define IP "127.0.0.1"
#define MAX_CONNECTIONS 10
#define MAX_CLIENTS 1000
#define TIMEOUT 100000
#define MAX_MSG_LENGTH 512

#define SOCK_ERR "Error: socket()"
#define FCNTL_ERR "Error: fcntl()"
#define LISTEN_ERR "Error: listen()"
#define BIND_ERR "Error: bind()"
#define POL_ERR "Error: poll()"
#define RECV_ERR "Error: recv()"
#define SEND_ERR "Error: send()"
#define SETSOCKOPT_ERR "Error: setsockopt()"

class server
{
private:
	/* data */
	int 						listenPort;
	int							listenerFd;

	int							fdCount;
	std::vector<struct pollfd>	clientSockets;

	void	checkStatusAndThrow(int, std::string) throw(std::runtime_error);
	struct pollfd createPollStruct(int fd, short events);
	void	makeFdNonBlock(int fd) throw(std::runtime_error);
	void	setupPoll();
	void	handleNewConnection();
	void	handleExistingConnection(int socketIndex);
	void	handshakeNewConnection(int clientFd) throw(std::runtime_error);
	int		acceptClient(int clientFd);

	/* function */
	void	createSocket() throw(std::runtime_error);
	void	changeSocketOpt() throw(std::runtime_error);
	void	bindSocket() throw(std::runtime_error);
	void	listenToSocket() throw(std::runtime_error);
	void	pollClients() throw(std::runtime_error);
	void	loopAndHandleConnections();
	

public:
	server(int port);
	~server();
};

// void	server::createSocket()
// {}

server::server(int port)
{
	listenPort = port;

	createSocket();
	makeFdNonBlock(listenerFd);
	changeSocketOpt();
	bindSocket();
	listenToSocket();
	setupPoll();
	std::cout << "Poll set" << std::endl;
	while (42)
	{
		pollClients();
		std::cout << "Polled clients" << std::endl;
		loopAndHandleConnections();
	}
}

server::~server()
{
}

void	server::checkStatusAndThrow(int exitCode, std::string msg) throw(std::runtime_error){
	if (exitCode < 0)
		throw std::runtime_error(msg);
}

struct pollfd server::createPollStruct(int fd, short events){
	struct pollfd temp;

	temp.fd = fd;
	temp.events = events;

	return temp;
}


void	server::createSocket() throw(std::runtime_error){
	listenerFd = socket(AF_INET, SOCK_STREAM, 0);
	checkStatusAndThrow(listenerFd, SOCK_ERR);
}

void	server::makeFdNonBlock(int fd) throw(std::runtime_error){
	int status;

	status = fcntl(fd, F_SETFL, O_NONBLOCK);
	checkStatusAndThrow(status, FCNTL_ERR);
}

void	server::changeSocketOpt() throw(std::runtime_error){
	int status;
	int opt = 1;

	status = setsockopt(this->listenerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	checkStatusAndThrow(status, SETSOCKOPT_ERR);
}

void		server::bindSocket() throw(std::runtime_error){
	int status;
	struct sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(IP);
	sockAddr.sin_port = htons(listenPort);

	status = bind(listenerFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
	checkStatusAndThrow(status, BIND_ERR);
}

void		server::listenToSocket() throw(std::runtime_error){
	int status;

	status = listen(listenerFd, MAX_CONNECTIONS);
	checkStatusAndThrow(status, LISTEN_ERR);
}

int		server::acceptClient(int clientFd){
	return (accept(clientFd, NULL, NULL));
}

void	server::setupPoll(){
	fdCount = 1;

	struct pollfd temp;
	temp.fd = listenerFd;
	temp.events = POLLIN;

	clientSockets.push_back(temp); //helper function has issues
}

void	server::pollClients() throw(std::runtime_error){
	int status;

	status = poll(&clientSockets[0], fdCount, -1);
	checkStatusAndThrow(status, POL_ERR);

}

void	server::handshakeNewConnection(int clientFd) throw(std::runtime_error){
	//change the message to be customizable to the specific user
	int status;
	status = send(clientFd, "CAP * ACK multi-prefix\r\n", strlen("CAP * ACK multi-prefix\r\n"), 0);
	checkStatusAndThrow(status, SEND_ERR);
	send(clientFd, "001 root :Welcome to the Internet Relay Network root\r\n", strlen("001 root :Welcome to the Internet Relay Network root\r\n"), 0);
	checkStatusAndThrow(status, SEND_ERR);
}

void	server::handleNewConnection(){
	std::cout << "New connection!!!" << std::endl;
	//new connection incomming
	int clientSocketFd;

	clientSocketFd = acceptClient(listenerFd);
	
	if (clientSocketFd == -1)
		std::cerr << "accept(): error" << std::endl;
	else
	{
		makeFdNonBlock(clientSocketFd);
		/* add client socket */
		struct pollfd temp;

		temp.fd = clientSocketFd;
		temp.events = POLLIN;

		clientSockets.push_back(temp);
		fdCount++;
		
		handshakeNewConnection(clientSocketFd);

		std::cout << "New client with fd: " << clientSocketFd << std::endl;
	}
}

void	server::handleExistingConnection(int socketIndex){
std::cout << "Old connection ;)" << std::endl;
	char buffer[MAX_MSG_LENGTH];
	//handle new connection
	int readBytes = recv(clientSockets[socketIndex].fd, buffer, sizeof(buffer), 0);
	if (readBytes == -1)
	{
		std::cerr << "recv()" << std::endl;
		//should I remove it or allow it to retry
		// clientSockets.erase(clientSockets.begin() + i);
		// fd_num--;
	}
	else if (readBytes == 0) //connection closed
	{
		std::cout << "Connection closed with fd: " << socketIndex << std::endl;
		clientSockets.erase(clientSockets.begin() + socketIndex);
		fdCount--;
		// close(i);
	}
	else
	{
		std::cout << buffer << std::endl;
	}
}

void	server::loopAndHandleConnections(){
	for(int i = 0; i <= fdCount; i++)
	{
		if (clientSockets[i].revents & POLLIN)
		{
			if (clientSockets[i].fd == listenerFd)
				handleNewConnection();
			else
				handleExistingConnection(i);
		}
	}
}

#endif