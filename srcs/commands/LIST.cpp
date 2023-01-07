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

void LIST::getSelectedChannels(const command& msg, const vector<channel>& channelList, stringstream& response) {
	string message = msg.getParameters().substr(0, msg.getParameters().length() - 1);
	string channel_name = message.substr(0, message.find(' '));

	for (vector<channel>::const_iterator it = channelList.begin(); it != channelList.end(); it++) {
		if (!('#' + it->getName()).compare(channel_name)) {
			response << RPL_LIST(msg.getClient().getServername(), msg.getClient().getNickname(), it->getName(),
			                     this->userCountToString(it), it->getChannelModes(), it->getTopic());
			break;
		}
	}
}

void LIST::getAllChannels(const command& msg, const vector<channel>& channelList, stringstream& response) {
	for (vector<channel>::const_iterator it = channelList.begin(); it != channelList.end(); it++) {
		response << RPL_LIST(msg.getClient().getServername(), msg.getClient().getNickname(), it->getName(),
		                     this->userCountToString(it), it->getChannelModes(), it->getTopic());
	}
}

string LIST::getList(const command& msg, const vector<channel>& channelList) {
	stringstream response;

	response << RPL_LISTSTART(msg.getClient().getServername(), msg.getClient().getNickname());
	*msg.getParameters().begin() == '\r' ? this->getAllChannels(msg, channelList, response) :
	                                       this->getSelectedChannels(msg, channelList, response);
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
