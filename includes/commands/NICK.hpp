#ifndef NICK_HPP
#define NICK_HPP

#include "user.hpp"
#include "channel.hpp"
#include "command.hpp"

#define ERR_NONICKNAMEGIVEN(servername, nickname) \
(":" + servername + " 431 * " + nickname + " :No nickname given\n")

#define ERR_ERRONEUSNICKNAME(servername, nickname) \
(":" + servername + " 432 * " + nickname + " :Erroneus nickname\n")

#define ERR_NICKNAMEINUSE(servername, nickname) \
(":" + servername + " 433 * " + nickname + " :Nickname is already in use\n")

#define RPL_NICK(old_nickname, new_nickname) \
(":" + old_nickname + " NICK " + new_nickname + "\n")


class NICK {

private:
	bool      isNicknameValid(const string& nickname) const;
	bool      isNicknameTaken(const vector<user>& users, const string& nickname) const;
	string    buildResponse(const command &msg, const vector<user>& userList, const string& new_nickname);

public:
	NICK();
	~NICK();

	string    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
