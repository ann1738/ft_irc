#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "reply.hpp"
#include "command.hpp"
#include "messageUtils.hpp"

class PRIVMSG {

private:
	messageUtils    m_utility;

	void      buildUserResponse(stringstream& response, const user& client, const vector<user>& users,
	                            const string& nickname, const string& message);
	void      buildChannelResponse(stringstream& response, const user& client, const vector<channel>& channels,
	                               const string& channel_name, const string& message);
	string    buildResponse(const command& msg, const vector<user>& users, const vector<channel>& channels,
	                        const string& recipient, const string& message);
	void      setDestination(const user& client, const vector<user>& users, const vector<channel>& channels,
	                         vector<reply>& ret, const string& recipient);

public:
	PRIVMSG();
	~PRIVMSG();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
