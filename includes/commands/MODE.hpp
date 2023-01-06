#ifndef MODE_HPP
#define MODE_HPP

#include <string>
#include <vector>
#include <sstream>
#include "channel.hpp"
#include "command.hpp"
// #include "reply.hpp"

using namespace std;

#define MODES "klvoisptnm"
#define MODES_WITH_ARGS "klvo"
#define MODES_WITHOUT_ARGS "isptnm"
#define MAX_USER_COUNT_LIMIT 1000000

#define RPL_CHANNELMODEIS(nickname, channel, mode, modeParams) \
("324 " + nickname + " #" + channel + " " + mode + " " + modeParams + "\n")

#define ERR_NEEDMOREPARAMS_MODE(servername, mode) \
(":" + servername + " 461 " + "MODE +" + mode + " Not enough parameters\n")

#define ERR_CHANOPRIVSNEEDED_MODE(servername, nickname, channel) \
(":" + servername + " 482 " + nickname + " " + channel + " :You're not channel operator\n")

#define ERR_NOSUCHCHANNEL_MODE(servername, nickname, channel) \
(":" + servername + " 403 " + nickname + " " + channel + " :No such channel\n")

#define ERR_NOSUCHNICK(servername, sender, recipient) \
(":" + servername + " 401 " + sender + " " + recipient + " :No such nick/channel\n")

#define ERR_PASSTOOSMALL(servername, nickname) \
(":" + servername + " " + nickname + " :Channel key must be at least 3 characters\n")

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

	user*			m_user;
	channel*		m_channel;
	string			m_reply;
	// vector<string>	m_reply_msg;

	size_t				modeArgsIndex;
	vector<string>		modeArgs;

	void	parseChannelName(string &parameters);
	void	parseModes(string &parameters);
	void	parseModeArguments(string &parameters);
	void	storeChannel(const string& channelName, vector<channel> &globalChannelList);
	void	storeUser(const string& channelName, vector<user> &globalUserList);

	bool					isChannel(const string& channelName, vector<channel> &globalChannelList);
	bool					isUserOperator(const user& User);
	vector<user>::iterator	findUser(vector<user> &userList, const string &nickname);

	void	redirectMode(vector<user> &globalUserList);

	string	handleModeS(bool isPlus);
	string	handleModeP(bool isPlus);
	string	handleModeI(bool isPlus);
	string	handleModeM(bool isPlus);
	string	handleModeN(bool isPlus);
	string	handleModeT(bool isPlus);
	string	handleModeL(bool isPlus);
	string	handleModeK(bool isPlus);
	string	handleModeO(bool isPlus, vector<user> &globalUserList);
	string	handleModeV(bool isPlus, vector<user> &globalUserList);

	void	clear();

	// vector<reply>	MODE::constructReply();

public:
	MODE();
	~MODE();

	string	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

#endif