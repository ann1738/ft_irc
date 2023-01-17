#include "NOTICE.hpp"

NOTICE::NOTICE()
{
}

NOTICE::~NOTICE()
{
}

string NOTICE::getRecipient(string& buffer) {
	string recipient = buffer.substr(0, buffer.find(':') - 1);
	buffer.erase(0, buffer.find(':'));
	return recipient;
}

bool NOTICE::isNicknameJustSpaces(const string& nickname) {
	return nickname.find_first_not_of(' ') == string::npos;
}

bool NOTICE::isNicknameInServer(const vector<user> &users, const string& nickname) {
	for (vector<user>::const_iterator it = users.begin(); it != users.end(); it++) {
		if (!it->getNickname().compare(nickname)) {
			return true;
		}
	}
	return false;
}

bool NOTICE::isNicknameInChannel(const vector<const user*> &users, const string& nickname) {
	for (vector<const user*>::const_iterator it = users.begin(); it != users.end(); it++) {
		if (!(*it)->getNickname().compare(nickname)) {
			return true;
		}
	}
	return false;
}

bool NOTICE::isRecipientAChannel(const string& recipient) {
	return !recipient.empty() && *recipient.begin() == '#';
}

// check if a channel exists & saves it for future use in the ::buildChannelResponse function
pair<bool, const vector<channel>::const_iterator> NOTICE::findChannel(const vector<channel>& channels, const string& channel_name) {
	for (vector<channel>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		if (!it->getName().compare(channel_name)) {
			return make_pair(true, it);
		}
	}
	return make_pair(false, channels.end());
}

bool NOTICE::canClientMessageChannel(const user& client, const channel& Channel) {
	// if the channel doesn't want to receive external messages (+n) & the client is not in the channel
	if (Channel.getNoExternalMsg() && !this->isNicknameInChannel(Channel.getUsers(), client.getNickname())) {
		return false;
	}

	// if the channel is moderated (+m) and the client is in the channel but not an operator (+o) or voiced (+v)
	if (Channel.getModerated()) {
		return !this->isNicknameInChannel(Channel.getUsers(), client.getNickname()) ? false :
		       Channel.isOperator(client) ? true :
		       Channel.isVoicedUser(client) ? true : false;
	}
	return true;
}

void NOTICE::buildUserResponse(stringstream& response, const user& client, const vector<user>& users,
                                const string& nickname, const string& message) {
	if (!this->isNicknameJustSpaces(nickname) && this->isNicknameInServer(users, nickname)) {
		response << RPL_NOTICE(client.getNickname(), nickname, message);
	}
}

void NOTICE::buildChannelResponse(stringstream& response, const user& client, const vector<channel>& channels,
                                   const string& channel_name, const string& message) {
	pair<bool, const vector<channel>::const_iterator> channel_info = this->findChannel(channels, &channel_name.at(1));

	if (!channel_info.first) {
		response << ERR_NOSUCHCHANNEL(client.getServername(), &channel_name.at(1));
		return;
	}
	response << (this->canClientMessageChannel(client, *channel_info.second) ? RPL_PRIVMSG(client.getNickname(), channel_name, message) :
	                                                                           ERR_CANNOTSENDTOCHAN(client.getServername(), &channel_name.at(1)));
}

string NOTICE::buildResponse(const command& msg, const vector<user>& users, const vector<channel>& channels,
                              const string& recipient, const string& message) {
	stringstream response;
	this->isRecipientAChannel(recipient) ? this->buildChannelResponse(response, msg.getClient(), channels, recipient, message) :
	                                       this->buildUserResponse(response, msg.getClient(), users, recipient, message);
	return response.str();
}

size_t  NOTICE::getChannelIndex(const vector<channel>& channels, string channel_name) {
	size_t i = 0;
	for (; i < channels.size(); i++) {
		if (channels.at(i).getName() == channel_name)
			break ;
	}
	return (i);
}

size_t  NOTICE::getUserIndex(const vector<user>& users, string nickname) {
	size_t i = 0;
	for (; i < users.size(); i++) {
		if (users.at(i).getNickname() == nickname)
			break ;
	}
	return (i);
}

void NOTICE::setDestination(const user& client, const vector<user>& users, const vector<channel>& channels,
                             vector<reply>& ret, const string& recipient) {
	ret.at(0).getMsg().find("NOTICE") == string::npos ? ret.at(0).setUserFds(client) :
	this->isRecipientAChannel(recipient) ? ret.at(0).setUserFds(channels.at(this->getChannelIndex(channels, &recipient.at(1))), client.getFd()) :
	                                       ret.at(0).setUserFds(users.at(this->getUserIndex(users, recipient)));
}

vector<reply> NOTICE::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	vector<reply> ret;
	ret.push_back(reply());
	
	string buffer = msg.getParameters(),
	       recipient = this->getRecipient(buffer),
	       message = buffer;

	ret.at(0).setMsg(this->buildResponse(msg, globalUserList, globalChannelList, recipient, message));
	this->setDestination(msg.getClient(), globalUserList, globalChannelList, ret, recipient);
	return ret;
}
