#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG()
{
}

PRIVMSG::~PRIVMSG()
{
}

void PRIVMSG::buildUserResponse(stringstream& response, const user& client, const vector<user>& users,
                                const string& nickname, const string& message) {
	response << (m_utility.isNicknameJustSpaces(nickname) ? ERR_NOTEXTTOSEND(client.getServername()) :
	            !m_utility.isNicknameInServer(users, nickname) ? ERR_NOSUCHNICK(client.getServername(), nickname) :
	                                                             RPL_PRIVMSG(client.getNickname(), nickname, message));
}

void PRIVMSG::buildChannelResponse(stringstream& response, const user& client, const vector<channel>& channels,
                                   const string& channel_name, const string& message) {
	pair<bool, const vector<channel>::const_iterator> channel_info = m_utility.findChannel(channels, &channel_name.at(1));

	if (!channel_info.first) {
		response << ERR_NOSUCHCHANNEL(client.getServername(), &channel_name.at(1));
		return;
	}

	response << (m_utility.canClientMessageChannel(client, *channel_info.second) ? RPL_PRIVMSG(client.getNickname(), channel_name, message) :
	                                                                               ERR_CANNOTSENDTOCHAN(client.getServername(), &channel_name.at(1)));
}

string PRIVMSG::buildResponse(const command& msg, const vector<user>& users, const vector<channel>& channels,
                              const string& recipient, const string& message) {
	stringstream response;
	m_utility.isRecipientAChannel(recipient) ? this->buildChannelResponse(response, msg.getClient(), channels, recipient, message) :
	                                           this->buildUserResponse(response, msg.getClient(), users, recipient, message);
	return response.str();
}

void PRIVMSG::setDestination(const user& client, const vector<user>& users, const vector<channel>& channels,
                             vector<reply>& ret, const string& recipient) {
	// an error has been encountered, destination fd will be the client
	if (ret.at(0).getMsg().find("PRIVMSG") == string::npos) {
		ret.at(0).setUserFds(client);
	}

	// message is meant for a channel, destination fds will be the fd of each user in the channel
	else if (m_utility.isRecipientAChannel(recipient)) {
		size_t i = m_utility.getChannelIndex(channels, &recipient.at(1));
		ret.at(0).setUserFds(channels.at(i), client.getFd());
	}

	// message is meant for a single client, destination fd will be that specific client
	else {
		ret.at(0).setUserFds(users.at(m_utility.getUserIndex(users, recipient)));
	}
}

vector<reply> PRIVMSG::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	vector<reply> ret;
	ret.push_back(reply());
	
	string buffer = msg.getParameters(),
	       recipient = m_utility.getRecipient(buffer),
	       message = buffer;

	ret.at(0).setMsg(this->buildResponse(msg, globalUserList, globalChannelList, recipient, message));
	this->setDestination(msg.getClient(), globalUserList, globalChannelList, ret, recipient);
	return ret;
}
