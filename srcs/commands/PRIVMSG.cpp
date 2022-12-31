#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG()
{
}

PRIVMSG::~PRIVMSG()
{
}

string PRIVMSG::getRecipient(string& buffer) {
	string recipient = buffer.substr(0, buffer.find(':') - 1);
	buffer.erase(0, buffer.find(':'));
	return recipient;
}

bool PRIVMSG::isNicknameJustSpaces(const string& nickname) const {
	return nickname.find_first_not_of(' ') == string::npos;
}

bool PRIVMSG::isUserInServer(const vector<user> &userList, const string& nickname) const {
	for (vector<user>::const_iterator it = userList.begin(); it != userList.end(); it++) {
		if (it->getNickname() == nickname) {
			return true;
		}
	}
	return false;
}

bool PRIVMSG::doesChannelExist(const vector<channel>& channelList, const string& channel_name) const {
	for (vector<channel>::const_iterator it = channelList.begin(); it != channelList.end(); it++) {
		if (("#" + it->getName()) == channel_name) {
			return true;
		}
	}
	return false;
}

bool PRIVMSG::isRecipientAChannel(const string& recipient) const {
	return !recipient.empty() && *recipient.begin() == '#';
}

void PRIVMSG::buildUserResponse(stringstream& response, const command &msg, const vector<user>& userList, 
                                const string& nickname, const string& message) const {
	if (this->isNicknameJustSpaces(nickname)) {
		response << ERR_NOTEXTTOSEND(msg.getClient().getServername());
	} else if (!this->isUserInServer(userList, nickname)) {
		response << ERR_NOSUCKNICK(msg.getClient().getServername(), nickname);
	} else {
		response << RPL_PRIVMSG(msg.getClient().getNickname(), nickname, message);
	}
}

void PRIVMSG::buildChannelResponse(stringstream& response, const command &msg, const vector<channel>& channelList, 
                                   const string& channel_name, const string& message) const {
	if (!this->doesChannelExist(channelList, channel_name)) {
		response << ERR_NOSUCHCHANNEL_(msg.getClient().getServername(), channel_name);
	} else {
		response << RPL_PRIVMSG(msg.getClient().getNickname(), channel_name, message);
	}

	// if a channel exists and its mode has 'm'
	// response << ERR_CANNOTSENDTOCHAN(msg.getClient().getServername(), channel_name);
}

string PRIVMSG::buildResponse(const command &msg, const vector<user>& userList, const vector<channel>& channelList,
                              const string& recipient, const string& message) const {
	stringstream response;
	this->isRecipientAChannel(recipient) ? this->buildChannelResponse(response, msg, channelList, recipient, message) :
	                                       this->buildUserResponse(response, msg, userList, recipient, message);
	return response.str();
}

string PRIVMSG::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	string buffer = msg.getParameters(),
	       recipient = this->getRecipient(buffer),
	       message = buffer;

	return this->buildResponse(msg, globalUserList, globalChannelList, recipient, message);
}
