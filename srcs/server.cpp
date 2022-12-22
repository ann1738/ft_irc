#include "../includes/server.hpp"

server::server(int port)
{
	listenPort = port;

	createSocket();
	// makeFdNonBlock(listenerFd);
	changeSocketOpt();
	bindSocket();
	listenToSocket();
	setupPoll();
	while (true)
	{
		pollClients();
		loopAndHandleConnections();
	}
}

server::~server()
{
}

void			server::checkStatusAndThrow(int exitCode, std::string msg) throw(std::runtime_error){
	if (exitCode < 0)
		throw std::runtime_error(msg);
}

struct pollfd	server::createPollStruct(int fd, short events){
	struct pollfd temp;

	temp.fd = fd;
	temp.events = events;

	return temp;
}

void			server::addSocket(int fd, short event){
	clientSockets.push_back(createPollStruct(fd, event));
	fdCount++;
}

void			server::removeSocket(int socketIndex){
	clientSockets.erase(clientSockets.begin() + socketIndex);
	fdCount--;
}


void			server::createSocket() throw(std::runtime_error){
	listenerFd = socket(AF_INET, SOCK_STREAM, 0);
	checkStatusAndThrow(listenerFd, SOCK_ERR);
}

void			server::makeFdNonBlock(int fd) throw(std::runtime_error){
	int status;

	status = fcntl(fd, F_SETFL, O_NONBLOCK);
	checkStatusAndThrow(status, FCNTL_ERR);
}

void			server::changeSocketOpt() throw(std::runtime_error){
	int status;
	int opt = 1;

	status = setsockopt(this->listenerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	checkStatusAndThrow(status, SETSOCKOPT_ERR);
}

void			server::bindSocket() throw(std::runtime_error){
	int status;
	struct sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_port = htons(listenPort);

	status = bind(listenerFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
	checkStatusAndThrow(status, BIND_ERR);
}

void			server::listenToSocket() throw(std::runtime_error){
	int status;

	status = listen(listenerFd, MAX_CONNECTIONS);
	checkStatusAndThrow(status, LISTEN_ERR);
}

int				server::acceptClient(int clientFd){
	return (accept(clientFd, NULL, NULL));
}

void			server::setupPoll(){
	fdCount = 1;
	clientSockets.push_back(createPollStruct(listenerFd, POLLIN));
}

void			server::pollClients() throw(std::runtime_error){
	int status;

	status = poll(&clientSockets[0], fdCount, TIMEOUT);
	checkStatusAndThrow(status, POL_ERR);

}

void			server::handshakeNewConnection(int clientFd) throw(std::runtime_error){
	//change the message to be customizable to the specific user
	int 		status;
	std::string msg;

	msg = "CAP * ACK multi-prefix\r\n";
	status = send(clientFd, msg.c_str(), msg.length(), 0);
	checkStatusAndThrow(status, SEND_ERR);

	// msg = this->createWelcomeMessage(this->findUserNickname(clientFd));
	// status = send(clientFd, msg.c_str(), msg.length(), 0);
	// checkStatusAndThrow(status, SEND_ERR);
}

void			server::handleNewConnection(){
	int clientSocketFd;

	clientSocketFd = acceptClient(listenerFd);
	if (clientSocketFd == -1)
		std::cerr << ACCEPT_ERR << std::endl;
	else
	{
		// makeFdNonBlock(clientSocketFd);
		addSocket(clientSocketFd, POLLIN);
		this->addUser(clientSocketFd);
		handshakeNewConnection(clientSocketFd);
	}
}

void			server::handleExistingConnection(int socketIndex){
	char	buffer[MAX_MSG_LENGTH] = {0};
	int		readBytes;
	
	readBytes = recv(clientSockets[socketIndex].fd, buffer, sizeof(buffer), 0);
	if (readBytes == -1)
	{
		std::cerr << "recv()" << std::endl;
		//should I remove it or allow it to retry
		// removeSocket(socketIndex);
	}
	else if (readBytes == 0) //connection closed
	{
		removeSocket(socketIndex);
	}
	else
	{
		std::cout << buffer << std::endl;
		int fd = clientSockets[socketIndex].fd;

		if (users[socketIndex - 1].getNickname().empty()) {
			users[socketIndex - 1].initNickname(users.size());
			std::string msg = this->createWelcomeMessage(users[socketIndex - 1].getNickname());
			send(fd, msg.c_str(), msg.length(), 0);
		}

		nick.changeNickname(buffer, users[socketIndex - 1].getNickname(), users[socketIndex - 1], fd);
	}
}

void			server::loopAndHandleConnections(){
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


/*-----------------------------------------------------------------------*/

void			server::addUser(int fd) {
	users.push_back(User(fd));
}

std::string		server::createWelcomeMessage(std::string nickname) {
	std::stringstream msg;
	msg << "001 " << nickname << " :Welcome to the Internet Relay Network " << nickname << "\r\n";
	return msg.str();
}
