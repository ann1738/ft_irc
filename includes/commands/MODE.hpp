#ifndef MODE_HPP
#define MODE_HPP

#include <string>
#include <vector>
#include <sstream>
// #define <cstdlib>
#include "channel.hpp"
#include "command.hpp"

using namespace std;

#define MODES "klvoisptnm"
#define MODES_WITH_ARGS "klvo"
#define MODES_WITHOUT_ARGS "isptnm"
#define MAX_USER_COUNT_LIMIT 1000000

#define ERR_NEEDMOREPARAMS_MODE(servername, mode) \
(":" + servername + " 461 " + "MODE +" + mode + " Not enough parameters\n")

#define RPL_CHANNELMODEIS(servername, nickname, channel, mode, modeParams) \
(":" +  servername + " 324 " + nickname + " " + channel + " " + mode + " " + modeParams + "\n")

#define ERR_CHANOPRIVSNEEDED_MODE(servername, nickname, channel) \
(":" + servername + " 482 " + nickname + " " + channel + " :You're not channel operator\n")

#define ERR_NOSUCHCHANNEL_MODE(servername, nickname, channel) \
(servername + " 403 " + nickname + " " + channel + " :No such channel\n")

/* ***	Supported Modes	*** */
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

/* ***	Not supported Modes	*** */
// b - set a ban mask to keep users out;

class MODE
{
private:
	string		parsedChannelName;
	string		parsedModes;

	user		m_user;
	channel*	m_channel;
	string		m_reply;

	size_t				modeArgsIndex;
	vector<string>		modeArgs;

	void	parseChannelName(string &parameters);
	void	parseModes(string &parameters);
	void	storeModeArguments(string &parameters);

	bool	isChannel(const string& channelName, vector<channel> &globalChannelList);
	void	storeChannel(const string& channelName, vector<channel> &globalChannelList);

	void	changeModes();

	bool	isUserOperator(const user& User);
	
	void	dealWithAppropriateMode();
	string	handleModeL(bool isPlus);

public:
	MODE();
	~MODE();

	string	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

#endif