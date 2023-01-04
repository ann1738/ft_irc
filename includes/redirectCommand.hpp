#ifndef REDIRECTCOMMAND_HPP
#define REDIRECTCOMMAND_HPP

#include <string>
#include "command.hpp"
#include "command_headers.hpp"

using namespace std;

class redirectCommand
{
private:
	/* IRC COMMANDS ARE DECLARED HERE */
	TOPIC topic;
	NICK nick;
	JOIN join;
	PART part;
	PRIVMSG privmsg;
	MODE mode;

public:
	redirectCommand();
	~redirectCommand();

	string	redirect(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

#endif
