#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "reply.hpp"
#include "command.hpp"

// #define ERR_NORECIPIENT
// #define ERR_TOOMANYTARGETS

class PRIVMSG {

private:
	string    getRecipient(string& buffer);
	bool      isNicknameJustSpaces(const string& nickname) const;
	bool      isUserInServer(const vector<user> &userList, const string& nickname) const;
	bool      isRecipientAChannel(const string& recipient) const;
	bool      doesChannelExist(const vector<channel>& channelList, const string& channel_name) const;

	void      buildUserResponse(stringstream& response, const command &msg, const vector<user>& userList,
	                            const string& nickname, const string& message) const;
	void      buildChannelResponse(stringstream& response, const command &msg, const vector<channel>& channelList,
	                               const string& channel_name, const string& message) const;
	string    buildResponseMsg(const command &msg, const vector<user>& userList, const vector<channel>& channelList,
	                        const string& recipient, const string& message) const;

	size_t    getChannelIndex(const vector<channel>& channelList, string channel_name);
	size_t    getUserIndex(const vector<user>& userList, string nickname);

public:
	PRIVMSG();
	~PRIVMSG();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
