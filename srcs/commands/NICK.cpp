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
 * checks every user's nickname in the server and returns true if the desired nickname is already
 * being used or false if the desired nickname is available. Furthermore, it also returns a
 * constant iterator to the user who owns the nickname
*/
pair<bool, vector<user>::const_iterator> NICK::isNicknameTaken(const vector<user>& userList, const string& nickname) const {
	for (vector<user>::const_iterator it = userList.begin(); it != userList.end(); it++) {
		if (!it->getNickname().compare(nickname)) {
			return make_pair(true, it);
		}
	}
	return make_pair(false, userList.end());
}

string NICK::buildResponse(const command &msg, const vector<user>& userList, const string& nickname) {
	stringstream response;

	/**
	 * prevents a nickname that only contains spaces
	 * example: /nick "   "
	*/
	if (nickname.find_first_not_of(' ') == string::npos) {
		response << ERR_NONICKNAMEGIVEN(msg.getClient().getServername());
	}

	/**
	 * checks if the desired nickname only consists of valid characters or
	 * if the parameters received from the client is equal to two
	 * example: /nick "hello world !"
	*/
	else if (!this->isNicknameValid(nickname)) {
		response << ERR_ERRONEUSNICKNAME(msg.getClient().getServername(), nickname);
	}

	/**
	 * if the owner of the nickname is requesting a change but gave the nickname it's
	 * currently using, ignore the command
	*/
	else if (this->isNicknameTaken(userList, nickname).first &&
	         this->isNicknameTaken(userList, nickname).second->getFd() == msg.getClient().getFd()) {
		;
	}

	// ensures that every user connected to the server will have a unique nickname
	else if (this->isNicknameTaken(userList, nickname).first) {
		response << ERR_NICKNAMEINUSE(msg.getClient().getServername(), nickname);
	}

	// change a user's nickname if no errors have been encountered
	else {
		response << RPL_NICK(msg.getClient().getNickname(), nickname);
		msg.getClient().setNickname(nickname);
	}
	return response.str();
}

vector<reply>    NICK::buildReplies(const command &msg, vector<user> &globalUserList, string response){
	vector<reply>	ret;
	size_t			i_ret = 0;

	if (response.find("NICK") != string::npos) {
		for (size_t i = 0; i < globalUserList.size(); i++){
			ret.push_back(reply());
			ret.at(i_ret).setMsg(response);
			ret.at(i_ret++).setUserFds(globalUserList.at(i));
		}
	}
	if (!i_ret) {
		ret.push_back(reply());
		ret.at(0).setMsg(response);
		ret.at(0).setUserFds(msg.getClient());
	}
	return (ret);
}

vector<reply> NICK::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	(void)globalChannelList;
	string nickname = msg.getParameters().substr(0, msg.getParameters().find('\r'));

	return this->buildReplies(msg, globalUserList, this->buildResponse(msg, globalUserList, nickname));
}
