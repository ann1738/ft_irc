#ifndef SERVER_HPP
# define SERVER_HPP

/*socket()*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>

class server
{
private:
	/* data */
	int	socketFd;

	/* function */
	void	createSocket();
	int		bindSocket();
	int		listenToSocket();
	void	acceptClient();	

public:
	server();
	~server();
};

// void	server::createSocket()
// {}

server::server(/* args */)
{
}

server::~server()
{
}


#endif