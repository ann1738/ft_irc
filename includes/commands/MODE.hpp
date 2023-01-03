#ifndef MODE_HPP
#define MODE_HPP

#include <string>
#include "channel.hpp"
#include "command.hpp"

/*	Supported Modes	*/
// o - give/take channel operator privileges;
// p - private channel flag;
// s - secret channel flag;
// i - invite-only channel flag;
// t - topic settable by channel operator only flag;
// n - no messages to channel from clients on the outside;
// m - moderated channel;
// l - set the user limit to channel;
// v - give/take the ability to speak on a moderated channel;
// k - set a channel key (password).

/*	Not supported Modes	*/
// b - set a ban mask to keep users out;

class MODE
{
private:
	string		parsedChannelName;
	string		parsedModes;
	string		plusModes;
	string		minusModes;

	channel*	m_channel;
	string		m_reply;

	void	parseChannelName(string &parameters);
	void	parseModes(string &parameters);

	bool	isChannel(const string& channelName);
	string	isMode(const string& modes);

	void	changeModes();

	void	constructReply();

public:
	MODE();
	~MODE();

	string	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

#endif