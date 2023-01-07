#include "LIST.hpp"

LIST::LIST()
{
}

LIST::~LIST()
{
}

string LIST::getChannelList(const command& msg, const vector<channel>& channelList) {
	stringstream response, user_count;

	response << RPL_LISTSTART(msg.getClient().getServername(), msg.getClient().getNickname());
	for (vector<channel>::const_iterator it = channelList.begin(); it != channelList.end(); it++) {
		user_count << it->getUserCount();
		response << RPL_LIST(msg.getClient().getServername(), msg.getClient().getNickname(), it->getName(),
		                     user_count.str(), it->getChannelModes(), it->getTopic());
		user_count.str(string());
	}
	response << RPL_LISTEND(msg.getClient().getServername(), msg.getClient().getNickname());
	return response.str();
}

vector<reply> LIST::buildResponse(const command& msg, const vector<channel>& channelList) {
	vector<reply> ret;

	ret.push_back(reply());
	ret[0].setUserFds(msg.getClient());
	ret[0].setMsg(this->getChannelList(msg, channelList));
	return ret;	
}

vector<reply> LIST::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	(void)globalUserList;

	return this->buildResponse(msg, globalChannelList);
}
