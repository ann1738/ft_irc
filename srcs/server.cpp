#include "../includes/server.hpp"
#include "channel.hpp"
#include "redirectCommand.hpp"

server::server(int port)
{
	listenPort = port;

	createSocket();
	changeSocketOpt();
	makeFdNonBlock(listenerFd);
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
	temp.revents = 0;

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
		makeFdNonBlock(clientSocketFd);
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
		std::cout << "s-------------------" << std::endl;
		std::cout << buffer << std::endl;
		std::cout << "e-------------------" << std::endl;
		
		int fd = clientSockets[socketIndex].fd;
		commandParse	parser;

		parser.parse(buffer, getUser(fd));
		users[socketIndex - 1].enterServer();
		users[socketIndex - 1].saveUserInfo(buffer);

		NICK nick;
		nick.doNickCommand(users, fd, buffer);

		redirectCommand	funnel;

		for (size_t i = 0; i < parser.getCommandAmount(); i++){
			string reply = funnel.redirect(parser.getParsedCmd(i), users, channels);
			this->sendToRecipient(fd, reply);
			cout << "******* sent reply start *******" << endl;
			cout << reply << endl;
			cout << "******* sent reply end *******" << endl;
		}
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
	users.push_back(user(fd));
}

user&		server::getUser(int fd){
	for (vector<user>::iterator iter = users.begin(); iter != users.end(); iter++) {
		if (iter->getFd() == fd) {
			return *iter;
		}
	}
	return users[0];
}

bool		server::isMessageForUser(const string& message) const {
	return message.find("PRIVMSG ") != string::npos;
}

bool		server::isMessageForChannel(const string& message) const {
	return message.find("PRIVMSG #") != string::npos;
}

vector<channel>::const_iterator	server::findChannel(const string& message) {
	if (message.empty()) {
		return channels.end();
	}

	string channel_name = message.substr(0, message.find(' '));
	for (vector<channel>::iterator it = channels.begin(); it != channels.end(); it++) {
		if (('#' + it->getName()) == channel_name) {
			return it;
		}
	}
	return channels.end();
}

void		server::sendToChannel(int senderFd, const string& message) {
	vector<channel>::const_iterator destination = this->findChannel(message.substr(message.find('#'), message.length()));
	if (destination == channels.end()) {
		return;
	}

	vector<user> userList = destination->getUsers();
	for (vector<user>::iterator it = userList.begin(); it != userList.end(); it++) {
		if (it->getFd() != senderFd) {
			send(it->getFd(), message.c_str(), message.length(), 0);
		}
	}
}

vector<user>::const_iterator	server::findUser(const string& message) {
	if (message.empty()) {
		return users.end();
	}

	string nickname = message.substr(message.find("PRIVMSG ") + 8, message.find(' ') - 1);
	for (vector<user>::iterator it = users.begin(); it != users.end(); it++) {
		if (it->getNickname() == nickname) {
			return it;
		}
	}
	return users.end();
}

void		server::sendToUser(const string& message) {
	vector<user>::const_iterator destination = this->findUser(message);
	if (destination != users.end()) {
		send(destination->getFd(), message.c_str(), message.length(), 0);
	}
}

/**
 * sender function typically used to inform a client that the command they attempted to use
 * encountered an error and thus will not be broadcasted to other clients in the server
*/
void		server::sendToSelf(int fd, const string& message) {
	send(fd, message.c_str(), message.length(), 0);
}

void		server::sendToRecipient(int senderFd, const string& message) {
	this->isMessageForChannel(message) ? this->sendToChannel(senderFd, message) :
	this->isMessageForUser(message) ? this->sendToUser(message) :
	                                  this->sendToSelf(senderFd, message);
}
