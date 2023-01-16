#include "user.hpp"

user::user()
{
}

user::~user()
{
}

user::user(int fd) : m_fd(fd),
                     m_username(""),
                     m_hostname(""),
                     m_servername(""),
                     m_realname(""),
                     m_nickname(""),
                     m_entered_server(false)
{
}

int user::getFd() const {
	return this->m_fd;
}

void user::setUsername(string username) {
	this->m_username = username;
}

void user::setHostname(string hostname) {
	this->m_hostname = hostname;
}

void user::setServername(string servername) {
	this->m_servername = servername;
}

void user::setRealname(string realname) {
	this->m_realname = realname.substr(1, realname.length() - 1);
}

void user::setNickname(string nickname) {
	this->m_nickname = nickname;
}

string user::getUsername() const {
	return this->m_username;
}

string user::getHostname() const {
	return this->m_hostname;
}

string user::getServername() const {
	return this->m_servername;
}

string user::getRealname() const {
	return this->m_realname;
}

string user::getNickname() const {
	return this->m_nickname;
}

void user::addChannel(const string& channel_name) {
	this->m_channels.push_back(channel_name);
}

void user::removeChannel(const string& channel_name) {
	for (vector<string>::iterator it = m_channels.begin(); it != m_channels.end(); it++) {
		if (*it == channel_name) {
			m_channels.erase(it);
			break ;
		}
	}
}

vector<string> user::parseMessage(char* buffer) const {
	stringstream temp(buffer);
	vector<string> client_message;
	string placeholder;

	while (temp >> placeholder) {
		client_message.push_back(placeholder);
	}
	return client_message;
}

void user::initializeNickname(const vector<string>& client_message) {
	vector<string>::const_iterator it = find(client_message.begin(), client_message.end(), "NICK");

	/**
	 * verify if the NICK has been provided and is not empty. Because if it was empty or just
	 * consisted of spaces, the next element in the vector would be a string "USER" and we wouldn't
	 * want to set our nickname to "USER"
	*/
	if (it != client_message.end() && client_message.at(it - client_message.begin() + 1).compare("USER")) {
		this->setNickname(client_message[it - client_message.begin() + 1]);
	}
}

bool user::isNicknameValid(const string& nickname) {
	string alpha("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
	       digits("0123456789"),
	       special("`|-_^{}[]\\");

	return nickname.length() < 10 &&
	       nickname.find_first_not_of(alpha + digits + special) == string::npos;
}

bool user::isNicknameTaken(const vector<user>& users, const string& nickname, int clientFd) {
	for (vector<user>::const_iterator it = users.begin(); it != users.end(); it++) {
		if (!it->getNickname().compare(nickname) && it->getFd() != clientFd) {
			return true;
		}
	}
	return false;
}

void user::saveUserInfo(const vector<string>& client_message) {
	vector<string>::const_iterator iter;

	if ((iter = find(client_message.begin(), client_message.end(), "USER")) != client_message.end()) {
		size_t i = iter - client_message.begin() + 1;
		this->setUsername(client_message[i++]);
		this->setHostname(client_message[i++]);
		this->setServername(SERVER_NAME);
		this->setRealname(client_message[++i]);
		this->m_entered_server = true;
	}
}

#include <iostream>
string user::enterServer(char *buffer, const vector<user>& users) {
	if (this->m_entered_server) {
		this->setServername(SERVER_NAME);
		return ERR_ALREADYREGISTRED(this->getServername());
	}

	vector<string> client_message = this->parseMessage(buffer);
	this->initializeNickname(client_message);

	if (find(client_message.begin(), client_message.end(), "NICK") != client_message.end() &&
	    find(client_message.begin(), client_message.end(), "USER") == client_message.end()) {
		return "";
	}

	if (this->getNickname().empty()) {
		return ERR_NONICKNAMEGIVEN(this->getServername());
	} else if (!this->isNicknameValid(this->getNickname())) {
		return ERR_ERRONEUSNICKNAME(this->getServername(), this->getNickname());
	} else if (this->isNicknameTaken(users, this->getNickname(), this->getFd())) {

		string message = "PING :" + this->getNickname() + "\n";
		send(this->getFd(), message.c_str(), message.length(), 0);
		// cout << "sent: " << message << endl;
		// if suspended

		// else
		return ERR_ALREADYREGISTRED(this->getServername());
	}

	this->saveUserInfo(client_message);
	cout << "USER = " << this->getUsername() << "." << endl;
	cout << "HOST = " << this->getHostname() << "." << endl;
	cout << "SERV = " << this->getServername() << "." << endl;
	cout << "REAL = " << this->getRealname() << "." << endl;
	return RPL_WELCOME(this->getNickname());
}
