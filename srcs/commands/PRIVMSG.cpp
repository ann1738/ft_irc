#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG()
{
}

PRIVMSG::~PRIVMSG()
{
}

string PRIVMSG::getNickname(string& buffer) {
	string nickname = buffer.substr(0, buffer.find(':') - 1);
	buffer.erase(0, buffer.find(':'));
	return nickname;
}

string PRIVMSG::getMessage(const string& buffer) const {
	vector<string>::size_type start = buffer.find(':'),
	                          end = buffer.length() - start - 1;
	return buffer.empty() ? "" : buffer.substr(start, end);
}

bool PRIVMSG::isNicknameJustSpaces(const string& nickname) const {
	for (size_t i = 0; i < nickname.size(); i++) {
		if (!isspace(nickname[i])) {
			return false;
		}
	}
	return true;
}

bool PRIVMSG::isUserInServer(const vector<user> &globalUserList, const string& nickname) const {
	for (vector<user>::const_iterator it = globalUserList.begin(); it != globalUserList.end(); it++) {
		if (it->getNickname() == nickname) {
			return true;
		}
	}
	return false;
}

string PRIVMSG::buildResponse(const command &msg, const vector<user> &globalUserList, string& nickname, string& message) const {
	stringstream response;

	if (this->isNicknameJustSpaces(nickname)) {
		response << ERR_NOTEXTTOSEND(msg.getClient().getServername());
	} else if (this->isUserInServer(globalUserList, nickname)) {
		response << ":" << msg.getClient().getNickname() << " PRIVMSG " << nickname << " " << message << "\n";
	} else {
		response << ERR_NOSUCKNICK(msg.getClient().getServername(), nickname);
	}
	// cout << response.str();
	return response.str();
}

/**
 * @REMINDER: add messaging functionality for channels once JOIN has been merged to the main branch
*/
string PRIVMSG::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	(void)globalChannelList;

	string buffer = msg.getParameters(),
	       nickname = this->getNickname(buffer),
	       message = this->getMessage(buffer);

	return this->buildResponse(msg, globalUserList, nickname, message);
}
