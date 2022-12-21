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

#define IP "127.0.0.1"
#define MAX_CONNECTIONS 10
#define MAX_CLIENTS 1000
#define TIMEOUT 100000
#define MAX_MSG_LENGTH 512

#define SOCK_ERR "Error: socket()"
#define FCNTL_ERR "Error: fcntl()"
#define LISTEN_ERR "Error: listen()"
#define BIND_ERR "Error: bind()"
#define POLL_ERR "Error: poll()"
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

	/* function */
	void	createSocket() throw(std::runtime_error);
	void	makeFdNonBlock(int fd) throw(std::runtime_error);
	void	changeSocketOpt() throw(std::runtime_error);
	int		bindSocket() throw(std::runtime_error);
	int		listenToSocket() throw(std::runtime_error);
	int		server::acceptClient(int clientFd);
	void	setupPoll();
	void	pollClients() throw(std::runtime_error);
	void	handshakeNewConnection() throw(std::runtime_error);
	void	loopAndHandleConnections() throw(std::runtime_error);
	void	handleNewConnection() throw(std::runtime_error);
	void	handleExistingConnection() throw(std::runtime_error);
	

public:
	server(int port);
	~server();
};

// void	server::createSocket()
// {}

server::server(int port)
{
	listenPort = port;
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


void	server::createSocket(){
	listenerFd = socket(AF_INET, SOCK_STREAM, 0);
	checkStatusAndThrow(listenerFd, SOCK_ERR);
}

void	server::makeFdNonBlock(int fd){
	int status;

	status = fcntl(fd, F_SETFL, O_NONBLOCK);
	checkStatusAndThrow(status, FCNTL_ERR);
}

void	server::changeSocketOpt(){
	int status;
	int opt = 1;

	status = setsockopt(this->listenerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	checkStatusAndThrow(status, SETSOCKOPT_ERR);
}

int		server::bindSocket(){
	int status;
	struct sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(IP);
	sockAddr.sin_port = htons(listenPort);

	status = bind(listenerFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
	checkStatusAndThrow(status, BIND_ERR);
}

int		server::listenToSocket(){
	int status;

	status = listen(listenerFd, MAX_CONNECTIONS);
	checkStatusAndThrow(status, LISTEN_ERR);
}

int		server::acceptClient(int clientFd){
	return (accept(clientFd, NULL, NULL));
}

void	server::setupPoll(){
	int fdCount = 1;
	std::vector<struct pollfd>	clientSockets;

	clientSockets.push_back(createPollStruct(listenerFd, POLLIN)); //helper function has issues
}

void	server::pollClients(){
	int status;

	status = poll(&clientSockets[0], fdCount, -1);
	checkStatusAndThrow(status, POLL_ERR);

}

void	server::loopAndHandleConnections() throw(std::runtime_error){
	for(int i = 0; i <= fdCount; i++)
	{
		if (clientSockets[i].revents & POLLIN)
		{
			if (clientSockets[i].fd == listenerFd)
				handleNewConnection();
			else
				handleExistingConnection();
		}
	}

}

void	server::handshakeNewConnection(){

}

void	server::handleNewConnection(){

}

void	server::handleExistingConnection(){

}


#endif