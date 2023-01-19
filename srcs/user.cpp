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
                     m_buffer(""),
					 m_entered_server(false),
					 m_authenticated(false) {
}

int user::getFd() const {
	return this->m_fd;
}

/**
 * assigns a "guest_n" nickname to a client that joined the server. Doing so will prevent
 * issues where clients use the same nickname.
*/
void user::initNickname(const string& buff) {
	size_t start = buff.find("NICK") + 5;
	size_t end = buff.find("\r\n", start);
	this->setNickname(buff.substr(start, (end - start)));
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

void user::saveUserInfo(char* buffer) {
	if (!this->getUsername().empty()) {
		return;
	}

	initNickname(buffer);

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

void user::setUsername(const string& username) {
	this->m_username = username;
}

void user::setHostname(const string& hostname) {
	this->m_hostname = hostname;
}

void user::setServername(const string& servername) {
	this->m_servername = servername;
}

void user::setRealname(const string& realname) {
	this->m_realname = realname.substr(1, realname.length() - 1);
}

void user::setNickname(const string& nickname) {
	this->m_nickname = nickname;
}
void user::setChannels(const vector<string>& channels){
	this->m_channels = channels;
}

vector<string> user::getChannels() const{
	return this->m_channels;
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

void user::addToBuffer(const string& message) {
	this->m_buffer += message;
}

string user::getBuffer() const {
	return this->m_buffer;
}

void user::clearBuffer() {
	this->m_buffer.clear();
}

size_t user::getChannelSize() const{
	return this->m_channels.size();
}

vector<string> user::getMsgHistory() const{
	return this->m_msg_history;
}

void user::addToMsgHistory(const string& msg){
	if (msg.find("PONG") == string::npos)
		this->m_msg_history.push_back(msg);
}

void user::setAuthenticate(const bool state){
	this->m_authenticated = state;
}

bool user::isAuthenticated() const{
	return this->m_authenticated;
}

bool user::isEnteredServer() const{
	return this->m_entered_server;
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

void user::enterServer() {
	if (!m_entered_server) {
		string message = RPL_WELCOME(this->getNickname()) + WEBAREBEARS + "\r\n";
		send(this->getFd(), message.c_str(), message.length(), 0);
		m_entered_server = true;
	}
}
