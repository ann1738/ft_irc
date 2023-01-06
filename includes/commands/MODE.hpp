#ifndef MODE_HPP
#define MODE_HPP

#include "command.hpp"
#include "reply.hpp"

#define MODES "klvoisptnm"
#define MODES_WITH_ARGS "klvo"
#define MODES_WITHOUT_ARGS "isptnm"
#define MAX_USER_COUNT_LIMIT 1000000

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
	string			parsedChannelName;
	string			parsedModes;

	user*			m_user;
	channel*		m_channel;
	vector<string>	m_reply_msg;

	size_t			modeArgsIndex;
	vector<string>	modeArgs;

	void	parseChannelName(string &parameters);
	void	parseModes(string &parameters);
	void	parseModeArguments(string &parameters);
	void	storeChannel(const string& channelName, vector<channel> &globalChannelList);
	void	storeUser(const string& channelName, vector<user> &globalUserList);

	bool					isChannel(const string& channelName, const vector<channel> &globalChannelList);
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

	vector<reply>	constructReply();

public:
	MODE();
	~MODE();

	vector<reply>	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

#endif