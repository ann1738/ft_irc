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

// #define ERR_CANNOTSENDTOCHAN
// #define ERR_NORECIPIENT
// #define ERR_TOOMANYTARGETS

class PRIVMSG {

private:
	void      extractNickname(vector<string>& nicknames, const string& buffer);
	string    getNickname(string& buffer);
	string    getMessage(const string& buffer) const;
	bool      isNicknameJustSpaces(const string& nickname) const;
	bool      isUserInServer(const vector<user> &globalUserList, const string& nickname) const;
	string    buildResponse(const command &msg, const vector<user> &globalUserList, string& nickname, string& message) const;

public:
	PRIVMSG();
	~PRIVMSG();

	string    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
