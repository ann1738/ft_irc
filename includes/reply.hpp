#ifndef REPLY_HPP
# define REPLY_HPP

#include "channel.hpp"
#include "user.hpp"

// ----- commands replies 
#define RPL_JOIN(nickname, channel_name) \
(":" + nickname + " JOIN #" + channel_name)

#define RPL_PART(client_nickname, channel_name) \
(":" + client_nickname + " PART #" + channel_name)

#define RPL_NICK(old_nickname, new_nickname) \
(":" + old_nickname + " NICK " + new_nickname + "\n")

#define RPL_PRIVMSG(sender, recipient, message) \
(":" + sender + " PRIVMSG " + recipient + " " + message + "\n")

#define RPL_TOPIC(servername, nickname, channel, topic) \
(":" + servername + " 332 " + nickname + " #" + channel + " :" + topic + "\n")
// -----

// ----- error replies
#define RPL_NOTOPIC(servername, nickname, channel) \
(":" + servername + " 331 * " + nickname  + " #" + channel + " :No topic is set" + "\n")

#define ERR_NOSUCKNICK(servername, nickname) \
(":" + servername + " 401 * " + nickname + " :No such nick/channel\n")

#define ERR_NOSUCHCHANNEL(servername, channel_name) \
(":" + servername + " 403 * " + channel_name + " :No such channel\n")

#define ERR_CANNOTSENDTOCHAN(servername, channel_name) \
(":" + servername + " 404 * " + channel_name + " :Cannot send to channel\n")

#define ERR_TOOMANYCHANNELS(server_name, client_name, channel_name) \
(":" + server_name + " 405 " + client_name + " #" + channel_name + " :You have joined too many channels")

#define ERR_NOTEXTTOSEND(servername) \
(":" + servername + " 412 * :No text to send\n")

#define ERR_NONICKNAMEGIVEN(servername) \
(":" + servername + " 431 * :No nickname given\n")

#define ERR_ERRONEUSNICKNAME(servername, nickname) \
(":" + servername + " 432 * " + nickname + " :Erroneus nickname\n")

#define ERR_NICKNAMEINUSE(servername, nickname) \
(":" + servername + " 433 * " + nickname + " :Nickname is already in use\n")

#define ERR_NOTONCHANNEL(servername, nickname, channel) \
(":" + servername + " 442 * " + nickname + " #" + channel + " :You're not on that channel\n")

#define ERR_NEEDMOREPARAMS(servername, nickname, cmd) \
(":" + servername + " 461 * " + nickname + " " + cmd + " :Not enough parameters\n")

#define ERR_CHANNELISFULL(server_name, nickname, channel_name) \
(":" + server_name + " 471 * " + nickname + " #" + channel_name + " :Cannot join channel (+l)")

#define ERR_INVITEONLYCHAN(server_name, nickname, channel_name) \
(":" + server_name + " 473 * " + nickname + " #" + channel_name + " :Cannot join channel (+i)")

#define ERR_BADCHANNELKEY(server_name, nickname, channel_name) \
(":" + server_name + " 475 * " + nickname + " #" + channel_name + " :Cannot join channel (+k)")

#define ERR_CHANOPRIVSNEEDED(servername, nickname, channel) \
(servername + " 482 * " + nickname + " #" + channel + " :You're not channel operator\n")
// -----

class reply {

private:
	vector<int>	userFds;
	string		msg;

public:
	reply();
	
	void	setUserFds(channel& c);
	void	setUserFds(user& u);
	void	setMsg(string m);

	vector<int>	getUserFds() const;
	string		getMsg() const;
	
	~reply();

};

#endif