#ifndef REDIRECTCOMMAND_HPP
#define REDIRECTCOMMAND_HPP

#include "command_headers.hpp"

class redirectCommand
{
private:
	/* IRC COMMANDS ARE DECLARED HERE */
	TOPIC topic;
	NICK nick;
	JOIN join;
	PART part;
	PRIVMSG privmsg;

public:
	redirectCommand();
	~redirectCommand();

	vector<reply>	redirect(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

#endif
