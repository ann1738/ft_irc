#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "reply.hpp"
#include "command.hpp"

class PRIVMSG {

private:
	string    getRecipient(string& buffer);
	bool      isNicknameJustSpaces(const string& nickname);
	bool      isNicknameInServer(const vector<user> &users, const string& nickname);
	bool      isNicknameInChannel(const vector<const user*> &users, const string& nickname);
	bool      isRecipientAChannel(const string& recipient);

	pair<bool, const vector<channel>::const_iterator>    findChannel(const vector<channel>& channels, const string& channel_name);
	bool      canClientMessageChannel(const user& client, const channel& channel);

	void      buildUserResponse(stringstream& response, const user& client, const vector<user>& users,
	                            const string& nickname, const string& message);
	void      buildChannelResponse(stringstream& response, const user& client, const vector<channel>& channels,
	                               const string& channel_name, const string& message);
	string    buildResponse(const command& msg, const vector<user>& users, const vector<channel>& channels,
	                        const string& recipient, const string& message);

	size_t    getChannelIndex(const vector<channel>& channelList, string channel_name);
	size_t    getUserIndex(const vector<user>& userList, string nickname);
	void      setDestination(const user& client, const vector<user>& users, const vector<channel>& channels,
	                         vector<reply>& ret, const string& recipient);

public:
	PRIVMSG();
	~PRIVMSG();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
