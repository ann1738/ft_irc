#ifndef LIST_HPP
#define LIST_HPP

#include "reply.hpp"
#include "command.hpp"

class LIST {

private:
	string           getChannelList(const command& msg, const vector<channel>& channelList);
	vector<reply>    buildResponse(const command& msg, const vector<channel> &channelList);

public:
	LIST();
	~LIST();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
