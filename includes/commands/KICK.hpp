#ifndef KICK_HPP
# define KICK_HPP

#include "reply.hpp"
#include "command.hpp"

class KICK{

private:

	string	kicked_client;
	string	channel_name;
	string	reason;

	void			parseParameters(const string& parameters);

	size_t			findUser(const vector<user> &globalUserList) const;
	size_t			findChannel(const vector<channel> &globalChannelList) const;

	string			goThroughErrors(const user& client, const vector<user> &globalUserList, const vector<channel> &globalChannelList) const;
	vector<reply>	doKickAction(const user& client, const vector<user> &globalUserList, vector<channel> &globalChannelList);

public:

	KICK();

	vector<reply>	execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

	~KICK();

};

#endif