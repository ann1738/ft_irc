#include "user.hpp"

user::user()
{
}

user::~user()
{
}

user::user(int fd) : m_fd(fd),
                     m_hostname(""),
                     m_nickname("")
                    //  m_mode("")
{
}

int user::getFd() {
	return this->m_fd;
}

/**
 * assigns a "guest_n" nickname to a client that joined the server. Doing so will prevent
 * issues where clients use the same nickname.
*/
void user::initNickname() {
	stringstream nickname;
	nickname << "guest_" << this->getFd() - 3;
	this->setNickname(nickname.str());
}

vector<string> user::parseMessage(char* buffer) {
	stringstream temp(buffer);
	vector<string> client_message;
	string placeholder;

	while (temp >> placeholder) {
		client_message.push_back(placeholder);
	}
	return client_message;
}

void user::saveUserInfo(char* buffer) {
	if (!this->getUsername().empty()) {
		return;
	}

	vector<string> client_message = this->parseMessage(buffer);
	vector<string>::iterator iter;
	if ((iter = find(client_message.begin(), client_message.end(), "USER")) == client_message.end()) {
		return;
	}

	size_t i = iter - client_message.begin() + 1;
	this->setUsername(client_message[i++]);
	this->setHostname(client_message[i++]);
	this->setServername(client_message[i++]);
	this->setRealname(client_message[i++]);
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

void user::addChannel(string channel_name) {
	this->m_channels.push_back(channel_name);
}
