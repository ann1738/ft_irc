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

bool PRIVMSG::doesChannelExist(const vector<channel>& globalChannelList, string& channel_name) const {
	for (vector<channel>::const_iterator it = globalChannelList.begin(); it != globalChannelList.end(); it++) {
		if (static_cast<string>("#" + it->getName()) == channel_name) {
			return true;
		}
	}
	return false;
}

bool PRIVMSG::isRecipientAChannel(const string& recipient) const {
	return !recipient.empty() && *recipient.begin() == '#';
}

void PRIVMSG::buildUserResponse(stringstream& response, const command &msg, const vector<user>& userList, 
                                string& nickname, string& message) const {
	if (this->isNicknameJustSpaces(nickname)) {
		response << ERR_NOTEXTTOSEND(msg.getClient().getServername());
	} else if (!this->isUserInServer(userList, nickname)) {
		response << ERR_NOSUCKNICK(msg.getClient().getServername(), nickname);
	} else {
		response << ":" << msg.getClient().getNickname() << " PRIVMSG " << nickname << " " << message << "\n";
	}
}

void PRIVMSG::buildChannelResponse(stringstream& response, const command &msg, const vector<channel>& channelList, 
	                               string& channel_name, string& message) const {
	if (!this->doesChannelExist(channelList, channel_name)) {
		response << ERR_NOSUCKNICK(msg.getClient().getServername(), channel_name);
	} else {
		response << ":" << msg.getClient().getNickname() << " PRIVMSG " << channel_name << " " << message << "\n";
	}

	// if a channel exists and its mode has 'm'
	// response << ERR_CANNOTSENDTOCHAN(msg.getClient().getServername(), channel_name);
}

string PRIVMSG::buildResponse(const command &msg, const vector<user>& userList, const vector<channel>& channelList,
                              string& recipient, string& message) const {
	stringstream response;
	this->isRecipientAChannel(recipient) ? this->buildChannelResponse(response, msg, channelList, recipient, message) :
	                                       this->buildUserResponse(response, msg, userList, recipient, message);
	return response.str();
}

string PRIVMSG::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	string buffer = msg.getParameters(),
	       recipient = this->getRecipient(buffer),
	       message = this->getMessage(buffer);

	return this->buildResponse(msg, globalUserList, globalChannelList, recipient, message);
}
