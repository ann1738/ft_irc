#ifndef INVITE_HPP
# define INVITE_HPP

#include "reply.hpp"
#include "command.hpp"

class INVITE {

private:

	string	invited_client;
	string	channel_name;

	void			parseParameters(const string& parameters);

	size_t			findUser(const vector<user> &globalUserList) const;
	size_t			findChannel(const vector<channel> &globalChannelList) const;

	string			goThroughErrors(const user& client, const vector<user> &globalUserList, const vector<channel> &globalChannelList) const;
	vector<reply>	doInviteAction(user& client, vector<user> &globalUserList, vector<channel> &globalChannelList);

public:
	INVITE();

	vector<reply>	execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

	~INVITE();

};

#endif