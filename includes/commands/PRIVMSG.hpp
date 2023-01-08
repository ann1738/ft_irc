#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "reply.hpp"
#include "command.hpp"

class PRIVMSG {

private:
	string    getRecipient(string& buffer);
	bool      isNicknameJustSpaces(const string& nickname);
	bool      isNicknameInList(const vector<user> &userList, const string& nickname);
	bool      isRecipientAChannel(const string& recipient);
	bool      doesChannelExist(const vector<channel>& channelList, const string& channel_name);
	channel    getChannel(const vector<channel>& channelList, const string& channel_name);

	void      buildUserResponse(stringstream& response, const command &msg, const vector<user>& userList,
	                            const string& nickname, const string& message);
	void      buildChannelResponse(stringstream& response, const command &msg, const vector<channel>& channelList,
	                               const string& channel_name, const string& message);
	string    buildResponseMsg(const command &msg, const vector<user>& userList, const vector<channel>& channelList,
	                        const string& recipient, const string& message);

	size_t    getChannelIndex(const vector<channel>& channelList, string channel_name);
	size_t    getUserIndex(const vector<user>& userList, string nickname);

public:
	PRIVMSG();
	~PRIVMSG();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
