#ifndef INVITE_HPP
# define INVITE_HPP

#include "reply.hpp"
#include "command.hpp"

class INVITE {

private:

	string	invited_client;
	string	channel_name;

	void			parseParameters(const string& parameters);

	size_t			findUser(vector<user> &globalUserList);
	size_t			findChannel(vector<channel> &globalChannelList);

	string			goThroughErrors(user& client, vector<user> &globalUserList, vector<channel> &globalChannelList);
	vector<reply>	doInviteAction(user& client, vector<user> &globalUserList, vector<channel> &globalChannelList);

public:
	INVITE();

	vector<reply>	execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

	~INVITE();

};

#endif