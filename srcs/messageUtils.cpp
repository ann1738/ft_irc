#include "messageUtils.hpp"

messageUtils::messageUtils()
{
}

messageUtils::~messageUtils()
{
}

string messageUtils::getRecipient(string& buffer) {
	string recipient = buffer.substr(0, buffer.find(':') - 1);
	buffer.erase(0, buffer.find(':'));
	return recipient;
}

bool messageUtils::isNicknameJustSpaces(const string& nickname) {
	return nickname.find_first_not_of(' ') == string::npos;
}

bool messageUtils::isNicknameInServer(const vector<user>& users, const string& nickname) {
	for (vector<user>::const_iterator it = users.begin(); it != users.end(); it++) {
		if (!it->getNickname().compare(nickname)) {
			return true;
		}
	}
	return false;
}

bool messageUtils::isNicknameInChannel(const vector<const user*>& users, const string& nickname) {
	for (vector<const user*>::const_iterator it = users.begin(); it != users.end(); it++) {
		if (!(*it)->getNickname().compare(nickname)) {
			return true;
		}
	}
	return false;
}

bool messageUtils::isRecipientAChannel(const string& recipient) {
	return !recipient.empty() && *recipient.begin() == '#';
}

// check if a channel exists & saves it for future use in the PRIVMSG && NOTICE::buildChannelResponse function
pair<bool, const vector<channel>::const_iterator> messageUtils::findChannel(const vector<channel>& channels, const string& channel_name) {
	for (vector<channel>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		if (!it->getName().compare(channel_name)) {
			return make_pair(true, it);
		}
	}
	return make_pair(false, channels.end());
}

bool messageUtils::canClientMessageChannel(const user& client, const channel& Channel) {
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

size_t messageUtils::getChannelIndex(const vector<channel>& channels, string channel_name) {
	size_t i = 0;
	for (; i < channels.size(); i++) {
		if (channels.at(i).getName() == channel_name)
			break ;
	}
	return (i);
}

size_t messageUtils::getUserIndex(const vector<user>& users, string nickname) {
	size_t i = 0;
	for (; i < users.size(); i++) {
		if (users.at(i).getNickname() == nickname)
			break ;
	}
	return (i);
}
