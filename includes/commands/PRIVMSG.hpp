#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "user.hpp"
#include "channel.hpp"
#include "command.hpp"
#include <algorithm>

#define ERR_NOSUCKNICK(servername, nickname) \
(":" + servername + " 401 * " + nickname + " :No such nick/channel\n")

#define ERR_NOTEXTTOSEND(servername) \
("No text to send\n")

#define ERR_CANNOTSENDTOCHAN(servername, channel_name) \
(":" + servername + " 404 * " + channel_name + " :Cannot send to channel\n")

// #define ERR_NORECIPIENT
// #define ERR_TOOMANYTARGETS

class PRIVMSG {

private:
	string    getRecipient(string& buffer);
	string    getMessage(const string& buffer) const;

	bool      isNicknameJustSpaces(const string& nickname) const;
	bool      isUserInServer(const vector<user> &globalUserList, const string& nickname) const;
	bool      isRecipientAChannel(const string& recipient) const;
	bool      doesChannelExist(const vector<channel>& globalChannelList, string& channel_name) const;

	void      buildUserResponse(stringstream& response, const command &msg, const vector<user>& userList, 
	                            string& nickname, string& message) const;
	void      buildChannelResponse(stringstream& response, const command &msg, const vector<channel>& channelList, 
	                               string& channel_name, string& message) const;
	string    buildResponse(const command &msg, const vector<user>& userList, const vector<channel>& channelList,
	                        string& recipient, string& message) const;

public:
	PRIVMSG();
	~PRIVMSG();

	string    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
