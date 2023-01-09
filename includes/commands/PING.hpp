#ifndef PING_HPP
#define PING_HPP

#include "reply.hpp"
#include "command.hpp"

class PING {

private:
	vector<reply>    buildResponse(const command &msg);

public:
	PING();
	~PING();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
