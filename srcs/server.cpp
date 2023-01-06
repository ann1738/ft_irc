#include "../includes/server.hpp"
#include "channel.hpp"
#include "redirectCommand.hpp"

server::server(int port) : listenPort(port),
                           stopServer(false) {
	createSocket();
	changeSocketOpt();
	makeFdNonBlock(listenerFd);
	bindSocket();
	listenToSocket();
	setupPoll();
}

void server::terminateServer() {
	stopServer = true;
}

void server::run() {
	while (!stopServer) {
		pollClients();
		loopAndHandleConnections();
	}
}

server::~server() {
	for (vector<struct pollfd>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++) {
		close(it->fd);
	}
	close(listenerFd);
	cout << "\nWeBareBears IRC Server has been terminated." << endl;
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
		parser.test();

		if (!this->isUserAuthenticated(users[socketIndex - 1])) {
			users[socketIndex - 1].enterServer();
			users[socketIndex - 1].saveUserInfo(buffer);
		} else {
			redirectCommand	funnel;
			for (size_t i = 0; i < parser.getCommandAmount(); i++){
				vector<reply> replies = funnel.redirect(parser.getParsedCmd(i), users, channels);
				this->sendReplies(replies);
			}
		}
	}
}

void			server::loopAndHandleConnections(){
	for(int i = 0; i < fdCount; i++)
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

// vector<channel>::const_iterator	server::findChannel(const string& message) {
// 	if (message.empty()) {
// 		return channels.end();
// 	}

// 	string channel_name = message.substr(message.find('#'), message.find(' '));
// 	if (channel_name.back() == '\n') {
// 		channel_name.resize(channel_name.length() - 1);
// 	}

// 	for (vector<channel>::iterator it = channels.begin(); it != channels.end(); it++) {
// 		if (('#' + it->getName()) == channel_name) {
// 			return it;
// 		}
// 	}
// 	return channels.end();
// }

// vector<user>::const_iterator	server::findUser(const string& message) {
// 	if (message.empty()) {
// 		return users.end();
// 	}

// 	string nickname = message.substr(message.find("PRIVMSG ") + 8, message.find(' ') - 1);
// 	for (vector<user>::iterator it = users.begin(); it != users.end(); it++) {
// 		if (it->getNickname() == nickname) {
// 			return it;
// 		}
// 	}
// 	return users.end();
// }

/**
 * checks if a user has been admitted to the server or not. This allows us to determine
 * if they may proceed in using the commands or if they should conclude the capability
 * negotiations first, followed by providing their information
*/
bool		server::isUserAuthenticated(const user& User) {
	// I think we could check the password around here

	return !User.getUsername().empty() &&
	       !User.getHostname().empty() &&
	       !User.getServername().empty() &&
	       !User.getRealname().empty();
}

void	server::sendReplies(const vector<reply>& replies){
	for (size_t reply_count = 0; reply_count < replies.size(); reply_count++) {
		for (size_t user_count = 0; user_count < replies[reply_count].getUserFds().size(); user_count++)
			send(replies[reply_count].getUserFds()[user_count], replies[reply_count].getMsg().c_str(), replies[reply_count].getMsg().length(), 0);
		cout << "******* sent reply start *******" << endl;
		cout << replies[reply_count].getMsg() << endl;
		cout << "******* sent reply end *******" << endl;
	}
}
