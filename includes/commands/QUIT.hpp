#ifndef QUIT_HPP
#define QUIT_HPP

#include "reply.hpp"
#include "command.hpp"

#define RPL_QUIT(channel, nickname, message) \
(":" + nickname + " QUIT :" + message + " - #" + channel +  "\n")

class QUIT {
private:
	string			parseQuitMsg(const string &paramaters);
	vector<reply>	constructReply(const command &msg, const string &parsedQuitMsg, vector<user> &globalUserList, vector<channel> &globalChannelList);

public:
	QUIT();
	~QUIT();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

#endif
