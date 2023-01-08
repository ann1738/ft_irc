#include "LIST.hpp"

LIST::LIST()
{
}

LIST::~LIST()
{
}

string LIST::userCountToString(vector<channel>::const_iterator& channel) {
	stringstream user_count;

	user_count << channel->getUserCount();
	return user_count.str();
}

vector<string> LIST::parseChannelNames(string& message) {
	vector<string> channels;

	string::size_type start = 0, end = message.find(',');
	while (end != string::npos) {
		channels.push_back(message.substr(start, end - start));
		start = end + 1;
		end = message.find(',', start);
	}

	string channel_name = message.substr(start, end);
	if (!channel_name.empty()) {
		channels.push_back(channel_name);
	}
	return channels;
}

bool LIST::doesChannelExist(const vector<string>& channels, const string& channel_name) {
	for (vector<string>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		if (!channel_name.compare(*it)) {
			return true;
		}
	}
	return false;
}

bool LIST::clientIsInsideChannel(const user& client, const vector<user>& userList) {
	for (vector<user>::const_iterator it = userList.begin(); it != userList.end(); it++) {
		if (client.getFd() == it->getFd()) {
			return true;
		}
	}
	return false;
}

string LIST::addTopic(vector<channel>::const_iterator it) {
	return it->getPrivate() ? "" : it->getTopic();
}

void LIST::addToResponse(const command& msg, vector<channel>::const_iterator it, stringstream& response) {
	if (it->getSecret() && !this->clientIsInsideChannel(msg.getClient(), it->getUsers())) {
		return;
	}
	response << RPL_LIST(msg.getClient().getServername(), msg.getClient().getNickname(), it->getName(),
		                 this->userCountToString(it), it->getChannelModes(), this->addTopic(it));
}

void LIST::addChannelsToList(const command& msg, const vector<channel>& channelList, stringstream& response) {
	string message = msg.getParameters().substr(0, msg.getParameters().length() - 1);
	vector<string> channels = this->parseChannelNames(message);

	for (vector<channel>::const_iterator it = channelList.begin(); it != channelList.end(); it++) {
		// if no parameters have been provided, print all the channels
		if (message.empty()) {
			this->addToResponse(msg, it, response);
		}

		// if specific channels have been provided, check if they exist and then print their information
		else if (!message.empty() && this->doesChannelExist(channels, '#' + it->getName())) {
			this->addToResponse(msg, it, response);
		}
	}
}

string LIST::getList(const command& msg, const vector<channel>& channelList) {
	stringstream response;

	response << RPL_LISTSTART(msg.getClient().getServername(), msg.getClient().getNickname());
	this->addChannelsToList(msg, channelList, response);
	response << RPL_LISTEND(msg.getClient().getServername(), msg.getClient().getNickname());
	return response.str();
}

vector<reply> LIST::buildResponse(const command& msg, const vector<channel>& channelList) {
	vector<reply> ret;

	ret.push_back(reply());
	ret[0].setUserFds(msg.getClient());
	ret[0].setMsg(this->getList(msg, channelList));
	return ret;	
}

vector<reply> LIST::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	(void)globalUserList;
	return this->buildResponse(msg, globalChannelList);
}
