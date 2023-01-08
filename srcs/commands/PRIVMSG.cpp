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

bool PRIVMSG::isNicknameJustSpaces(const string& nickname) {
	return nickname.find_first_not_of(' ') == string::npos;
}

/**
 * a helper function used to check if a given nickname is included in a list of nicknames. These
 * lists could either represent a list of users in the server, or a list of users in a channel
*/
bool PRIVMSG::isNicknameInList(const vector<user> &users, const string& nickname) {
	for (vector<user>::const_iterator it = users.begin(); it != users.end(); it++) {
		if (!it->getNickname().compare(nickname)) {
			return true;
		}
	}
	return false;
}

bool PRIVMSG::isRecipientAChannel(const string& recipient) {
	return !recipient.empty() && *recipient.begin() == '#';
}

// check if a channel exists & saves it for future use in the ::buildChannelResponse function
pair<bool, const vector<channel>::const_iterator> PRIVMSG::findChannel(const vector<channel>& channels, const string& channel_name) {
	for (vector<channel>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		if (!it->getName().compare(channel_name)) {
			return make_pair(true, it);
		}
	}
	return make_pair(false, channels.end());
}

bool PRIVMSG::canClientMessageChannel(const user& client, const channel& Channel) {
	// if the channel doesn't want to receive external messages (+n) & the client is not in the channel
	if (Channel.getNoExternalMsg() && !this->isNicknameInList(Channel.getUsers(), client.getNickname())) {
		return false;
	}

	// if the channel is moderated (+m) and the client is in the channel but not an operator (+o) or voiced (+v)
	else if (Channel.getModerated()) {
		return !this->isNicknameInList(Channel.getUsers(), client.getNickname()) ? false :
		       Channel.isOperator(client) ? true :
		       Channel.isVoicedUser(client) ? true : false;
	}
	return true;
}

void PRIVMSG::buildUserResponse(stringstream& response, const user& client, const vector<user>& users,
                                const string& nickname, const string& message) {
	response << (this->isNicknameJustSpaces(nickname) ? ERR_NOTEXTTOSEND(client.getServername()) :
	            !this->isNicknameInList(users, nickname) ? ERR_NOSUCHNICK(client.getServername(), nickname) :
	                                                       RPL_PRIVMSG(client.getNickname(), nickname, message));
}

void PRIVMSG::buildChannelResponse(stringstream& response, const user& client, const vector<channel>& channels,
                                   const string& channel_name, const string& message) {
	pair<bool, const vector<channel>::const_iterator> channel_info = this->findChannel(channels, &channel_name.at(1));

	if (!channel_info.first) {
		response << ERR_NOSUCHCHANNEL(client.getServername(), channel_name);
		return;
	}
	response << (this->canClientMessageChannel(client, *channel_info.second) ? RPL_PRIVMSG(client.getNickname(), channel_name, message) :
	                                                                           ERR_CANNOTSENDTOCHAN(client.getServername(), channel_name));
}

string PRIVMSG::buildResponse(const command& msg, const vector<user>& users, const vector<channel>& channels,
                              const string& recipient, const string& message) {
	stringstream response;
	this->isRecipientAChannel(recipient) ? this->buildChannelResponse(response, msg.getClient(), channels, recipient, message) :
	                                       this->buildUserResponse(response, msg.getClient(), users, recipient, message);
	return response.str();
}

size_t  PRIVMSG::getChannelIndex(const vector<channel>& channelList, string channel_name) {
	size_t i = 0;
	for (; i < channelList.size(); i++) {
		if (channelList[i].getName() == channel_name)
			break ;
	}
	return (i);
}

size_t  PRIVMSG::getUserIndex(const vector<user>& userList, string nickname) {
	size_t i = 0;
	for (; i < userList.size(); i++) {
		if (userList[i].getNickname() == nickname)
			break ;
	}
	return (i);
}

vector<reply> PRIVMSG::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	vector<reply> r;
	r.push_back(reply());
	
	string buffer = msg.getParameters(),
	       recipient = this->getRecipient(buffer),
	       message = buffer;

	r[0].setMsg(this->buildResponse(msg, globalUserList, globalChannelList, recipient, message));
	if (r[0].getMsg().find("PRIVMSG") == string::npos)
		r[0].setUserFds(msg.getClient());
	else if (this->isRecipientAChannel(recipient))
		r[0].setUserFds(globalChannelList[getChannelIndex(globalChannelList, &recipient[1])], msg.getClient().getFd());
	else
		r[0].setUserFds(globalUserList[getUserIndex(globalUserList, recipient)]);
	
	return r;
}
