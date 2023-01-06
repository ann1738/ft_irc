#include "PING.hpp"

PING::PING()
{
}

PING::~PING()
{
}

vector<reply>	PING::buildResponse(const command &msg) {
	vector<reply> ret;

	ret.push_back(reply());
	ret[0].setUserFds(msg.getClient());
	ret[0].setMsg(RPL_PING(msg.getClient().getServername(), msg.getParameters()));
	return ret;
}

vector<reply>	PING::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	(void)globalUserList;
	(void)globalChannelList;

	return this->buildResponse(msg);
}
