#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

#include "reply.hpp"
#include "channel.hpp"
#include "user.hpp"
#include "commandParse.hpp"

/* -------------- Macros --------------- */
#define TIMEOUT 100000
#define MAX_CONNECTIONS 10
#define MAX_MSG_LENGTH 512

/* ------------ Error messages ------------ */
#define SOCK_ERR "Error: socket()"
#define FCNTL_ERR "Error: fcntl()"
#define LISTEN_ERR "Error: listen()"
#define BIND_ERR "Error: bind()"
#define POL_ERR "Error: poll()"
#define RECV_ERR "Error: recv()"
#define SEND_ERR "Error: send()"
#define ACCEPT_ERR "Error: accept()"
#define SETSOCKOPT_ERR "Error: setsockopt()"

class server
{
private:
	int 						listenPort;
	int							listenerFd;
	bool						stopServer;

	int							fdCount;
	std::vector<struct pollfd>	clientSockets;

	void			addSocket(int fd, short event);
	void			removeSocket(int socketIndex);
	void			checkStatusAndThrow(int exitCode, std::string msg) throw(std::runtime_error);
	struct pollfd	createPollStruct(int fd, short events);

	void			makeFdNonBlock(int fd) throw(std::runtime_error);
	void			setupPoll();
	void			handleNewConnection();
	void			handleExistingConnection(int clientFd);
	int				acceptClient(int clientFd);
	void			handshakeNewConnection(int clientFd) throw(std::runtime_error);

	void			createSocket() throw(std::runtime_error);
	void			changeSocketOpt() throw(std::runtime_error);
	void			bindSocket() throw(std::runtime_error);
	void			listenToSocket() throw(std::runtime_error);
	void			pollClients() throw(std::runtime_error);
	void			loopAndHandleConnections();

	/*-----------------------------------------------------------------------*/

	std::vector<user>			users;
	std::vector<channel>		channels;

	void			addUser(int fd);
	user&			getUser(int fd);
	bool			isUserAuthenticated(const user& User);

	void			sendReplies(const vector<reply>& replies);

public:
	server(int port);
	~server();

	void			terminateServer();
	void			run();
};



#endif
