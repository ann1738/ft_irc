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

// splits the channel names based on ',' (if there are any), and then save them inside a vector
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

bool LIST::isClientInsideChannel(const vector<user>& userList, const user& client) {
	for (vector<user>::const_iterator it = userList.begin(); it != userList.end(); it++) {
		if (client.getFd() == it->getFd()) {
			return true;
		}
	}
	return false;
}

// returns a string containing all of the modes a channel has enabled
string LIST::addMode(vector<channel>::const_iterator it) {
	return it->getChannelModes().empty() ? "" : ("+" + it->getChannelModes());
}

/**
 * returns a string containing the topic of a channel. However if the channel is private (+p),
 * an empty string will be returned
*/
string LIST::addTopic(vector<channel>::const_iterator it) {
	return it->getPrivate() ? "" : it->getTopic();
}

// adds a channel's information to the response stringstream
void LIST::addToResponse(const command& msg, vector<channel>::const_iterator it, stringstream& response) {
	/**
	 * if a channel is secret (+s), and the client requesting a list of channels has not joined that
	 * specific channel, dont include it to the list
	*/
	if (it->getSecret() && !this->isClientInsideChannel(it->getUsers(), msg.getClient())) {
		return;
	}
	response << RPL_LIST(msg.getClient().getServername(), msg.getClient().getNickname(), it->getName(),
		                 this->userCountToString(it), this->addMode(it), this->addTopic(it));
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
