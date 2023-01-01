#include "NICK.hpp"

NICK::NICK()
{
}

NICK::~NICK()
{
}

/**
 * checks if the desired nickname only consists of valid characters and if its length
 * does not exceed 9 characters
*/
bool NICK::isNicknameValid(const string& nickname) const {
	string alpha("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
	       digits("0123456789"),
	       special("`|-_^{}[]\\");

	return nickname.length() < 10 &&
	       nickname.find_first_not_of(alpha + digits + special) == string::npos;
}

/**
 * checks every user's nickname in the server and returns true if the desired nickname is
 * already being used or false if the desired nickname is available for use
*/
bool NICK::isNicknameTaken(const vector<user>& userList, const string& nickname) const {
	for (vector<user>::const_iterator it = userList.begin(); it != userList.end(); it++) {
		if (!it->getNickname().compare(nickname)) {
			return true;
		}
	}
	return false;
}

string NICK::buildResponse(const command &msg, const vector<user>& userList, const string& nickname) {
	stringstream response;

	/**
	 * prevents a nickname that only contains spaces
	 * example: /nick "   "
	*/
	if (nickname.find_first_not_of(' ') == string::npos) {
		response << ERR_NONICKNAMEGIVEN(msg.getClient().getServername(), nickname);
	}

	/**
	 * checks if the desired nickname only consists of valid characters or
	 * if the parameters received from the client is equal to two
	 * example: /nick "hello world !"
	*/
	else if (!this->isNicknameValid(nickname)) {
		response << ERR_ERRONEUSNICKNAME(msg.getClient().getServername(), nickname);
	}

	// ensures that every user connected to the server will have a unique nickname
	else if (this->isNicknameTaken(userList, nickname)) {
		response << ERR_NICKNAMEINUSE(msg.getClient().getServername(), nickname);
	}

	// change a user's nickname if no errors have been encountered
	else {
		response << RPL_NICK(msg.getClient().getNickname(), nickname);
		msg.getClient().setNickname(nickname);
	}
	return response.str();
}

string NICK::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	(void)globalChannelList;

	string nickname = msg.getParameters().substr(0, msg.getParameters().find('\r'));
	return this->buildResponse(msg, globalUserList, nickname);
}
