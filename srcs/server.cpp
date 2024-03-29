#include "server.hpp"

server::server(int port, const string& password) : listenPort(port),
                        					stopServer(false),
											serverPassword(password) {
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

void			server::removeSocket(int fd){
	for (std::vector<struct pollfd>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++){
		if (it->fd == fd)
		{
			clientSockets.erase(it);
			fdCount--;
			break ;
		}
	}
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
	int 		status;
	std::string msg;

	msg = "CAP * ACK multi-prefix\r\n";
	status = send(clientFd, msg.c_str(), msg.length(), 0);
	checkStatusAndThrow(status, SEND_ERR);
}

void			server::handleNewConnection(){
	int clientSocketFd;

	clientSocketFd = acceptClient(listenerFd);
	if (clientSocketFd == -1)
		std::cerr << ACCEPT_ERR << std::endl;
	else
		addUserToServer(clientSocketFd);
}

void			server::handleExistingConnection(int clientFd){
	char	buffer[MAX_MSG_LENGTH] = {0};
	int		readBytes;

	readBytes = recv(clientFd, buffer, sizeof(buffer), 0);
	if (readBytes == -1)
	{
		std::cerr << "recv()" << std::endl;
		//should I remove it or allow it to retry
		// removeSocket(socketIndex);
	}
	else if (readBytes == 0) //connection closed
	{
		removeUserFromServer(clientFd);
	}
	else
	{
		std::cout << "s-------------------" << std::endl;
		std::cout << buffer << std::endl;
		std::cout << "e-------------------" << std::endl;
	
		parser.parse(buffer, getUser(clientFd));
		parser.test();

		if (!getUser(clientFd).isEnteredServer()) {
			try {
				if (!getUser(clientFd).isAuthenticated()) {
					authenticateProcess(clientFd, buffer);
				}
				else {
					if (parser.getParsedCmd(0).getCmdType() == "PONG") {
						string	msg = ":WeBareBears 462 * :You may not reregister\n";
						int s = send(clientFd, msg.c_str(), msg.length(), 0);
						checkStatusAndThrow(s, SEND_ERR);
						removeUserFromServer(clientFd);
					}
					else
						reconnect(clientFd);
				}
			} catch (exception &e) {
				cout << e.what() << endl;
			}
		} else {
			redirectCommand	funnel;
			for (size_t i = 0; i < parser.getCommandAmount(); i++){
				try{
					vector<reply> replies = funnel.redirect(parser.getParsedCmd(i), users, channels);
					if (parser.getParsedCmd(i).getCmdType() == "QUIT")
						removeUserFromServer(clientFd);
					this->sendReplies(replies);
				} catch(std::exception &e) {
					cout << e.what() << endl;
				}
			}
		}
		logUsers();
		logChannels();
		cout << endl << CYAN << "The amount of fds: " << WHITE << fdCount << endl;
	}
}

void			server::reconnect(int clientFd){
	for (size_t i = 0; i < users.size(); i++){
		if ((users[i].getFd() != getUser(clientFd).getFd()) && (users[i].getNickname() == getUser(clientFd).getNickname())){
			getUser(clientFd).setUsername(users[i].getUsername());
			getUser(clientFd).setHostname(users[i].getHostname());
			getUser(clientFd).setServername(users[i].getServername());
			getUser(clientFd).setRealname(users[i].getRealname());
			getUser(clientFd).setChannels(users[i].getChannels());
			for (size_t index = 0; index < users[i].getMsgHistory().size(); index++)
				getUser(clientFd).addToMsgHistory(users[i].getMsgHistory()[index]);
			getUser(clientFd).enterServer();
			for (vector<channel>::iterator it = channels.begin(); it != channels.end(); it++)
			{
				if (it->isUser(users[i]))
				{
					bool isO = it->isOperator(users[i]);
					bool isV = it->isVoicedUser(users[i]);
					bool isI = it->isInvitedUser(users[i]);
					it->removeUser(users[i]);
					it->addUser(getUser(clientFd));
					if (isO) it->addOperator(getUser(clientFd));
					if (isV) it->addVoicedUser(getUser(clientFd));
					if (isI) it->addInvitedUser(getUser(clientFd));
				}
			}
			for (size_t index = 0; index < getUser(clientFd).getMsgHistory().size(); index++){
				int s = send(clientFd, getUser(clientFd).getMsgHistory()[index].c_str(), getUser(clientFd).getMsgHistory()[index].length(), 0);
				checkStatusAndThrow(s, SEND_ERR);
			}
			removeUserFromServer(users[i].getFd());
			break ;
		}
	}
}

void			server::authenticateProcess(const int clientFd, char* buff){
	authenticate	a(parser, this->serverPassword);
	if (a.isAuthenticated() == AUTHENTICATED){
		if (getUser(clientFd).saveUserInfo(buff) == false) {
			send(clientFd, getUser(clientFd).getErrorMsg().c_str(), getUser(clientFd).getErrorMsg().length(), 0);
			this->removeUserFromServer(clientFd);
			return;
		}

		getUser(clientFd).setAuthenticate(true);
		if (isNicknameUnique(clientFd))
			getUser(clientFd).enterServer();
		else {
			string s = "PING " + getUser(clientFd).getNickname() + "\n";
			send(clientFd, s.c_str(), s.length(), 0);
		}
	}
	else if (a.isAuthenticated() == NOT_AUTHENTICATED){
		send(clientFd, a.getErrorMsg().c_str(), a.getErrorMsg().length(), 0);
		removeUserFromServer(clientFd);
	}
}

bool			server::isNicknameUnique(int clientFd){
	for (size_t i = 0; i < users.size(); i++){
		if ((users[i].getFd() != getUser(clientFd).getFd()) && (users[i].getNickname() == getUser(clientFd).getNickname()))
			return (false);
	}
	return (true);
}

void			server::loopAndHandleConnections(){
	for(int i = 0; i < fdCount; i++)
	{
		if (clientSockets[i].revents & POLLIN)
		{
			if (clientSockets[i].fd == listenerFd)
				handleNewConnection();
			else
				handleExistingConnection(clientSockets[i].fd);
		}
	}

}

/*-----------------------------------------------------------------------*/

bool			server::addUserToVector(int fd) {
	if (users.empty()) {
		users.reserve(MAX_CONNECTIONS);
	}

	if (users.size() == MAX_CONNECTIONS) {
		close(fd);
		return false;
	}
	users.push_back(user(fd));
	return true;
}

void			server::addUserToServer(int fd) {
	makeFdNonBlock(fd);
	addSocket(fd, POLLIN);
	if (addUserToVector(fd)) {
		handshakeNewConnection(fd);
	}
}

void			server::removeUserFromVector(int fd){
	for (vector<user>::iterator it = users.begin(); it != users.end(); it++) {
		if (it->getFd() == fd)
		{
			users.erase(it);
			break ;
		}
	}
}

void			server::removeUserFromServer(int fd){
	removeSocket(fd);
	removeUserFromVector(fd);
	close(fd);
}

user&		server::getUser(int fd){
	for (vector<user>::iterator iter = users.begin(); iter != users.end(); iter++) {
		if (iter->getFd() == fd) {
			return *iter;
		}
	}
	return users[0];
}

void		server::sendReplies(const vector<reply>& replies){
	for (size_t reply_count = 0; reply_count < replies.size(); reply_count++) {
		for (size_t user_count = 0; user_count < replies[reply_count].getUserFds().size(); user_count++){
			getUser(replies[reply_count].getUserFds()[user_count]).addToMsgHistory(replies[reply_count].getMsg());
			send(replies[reply_count].getUserFds()[user_count], replies[reply_count].getMsg().c_str(), replies[reply_count].getMsg().length(), 0);
		}
		cout << "******* sent reply start *******" << endl;
		cout << replies[reply_count].getMsg() << endl;
		cout << "******* sent reply end *******" << endl;
	}
}

void		server::logUsers() const{
	cout << endl << PURPLE << "List of users in the server:" << WHITE << endl;
	for (vector<user>::const_iterator it = users.begin(); it != users.end(); it++){
		cout << it->getNickname() << endl;
	}
	cout << PURPLE <<  "End of list" << WHITE << endl;
}

void		server::logChannels() const{
	cout << endl << PURPLE <<  "List of channels in the server:" << WHITE << endl;
	for (vector<channel>::const_iterator it = channels.begin(); it != channels.end(); it++){
		cout << it->getName() << endl;
	}
	cout << PURPLE <<  "End of list" << WHITE << endl;
}
