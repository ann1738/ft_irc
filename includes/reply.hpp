#ifndef REPLY_HPP
# define REPLY_HPP

#include "channel.hpp"
#include "user.hpp"

// ----- join 
#define JOIN_CORRECT(client_nickname, channel_name) \
(":" + client_nickname + " JOIN #" + channel_name)

#define ERR_INVITEONLYCHAN(server_name, client_name, channel_name) \
(":" + server_name + " 473 " + client_name + " #" + channel_name + " :Cannot join channel (+i)")

#define ERR_BADCHANNELKEY(server_name, client_name, channel_name) \
(":" + server_name + " 475 " + client_name + " #" + channel_name + " :Cannot join channel (+k)")

#define ERR_CHANNELISFULL(server_name, client_name, channel_name) \
(":" + server_name + " 471 " + client_name + " #" + channel_name + " :Cannot join channel (+l)")

// #define ERR_TOOMANYCHANNELS(server_name, client_name, channel_name) \
// (":" + server_name + " 405 " + client_name + " #" + channel_name + " :You have joined too many channels")

// -----

// ----- nick
#define ERR_NONICKNAMEGIVEN(servername) \
(":" + servername + " 431 * :No nickname given\n")

#define ERR_ERRONEUSNICKNAME(servername, nickname) \
(":" + servername + " 432 * " + nickname + " :Erroneus nickname\n")

#define ERR_NICKNAMEINUSE(servername, nickname) \
(":" + servername + " 433 * " + nickname + " :Nickname is already in use\n")

#define RPL_NICK(old_nickname, new_nickname) \
(":" + old_nickname + " NICK " + new_nickname + "\n")
// -----

// ----- part
#define PART_CORRECT(client_nickname, channel_name) \
(":" + client_nickname + " PART #" + channel_name)

#define P_ERR_NOSUCHCHANNEL(server_name, client_name, channel_name) \
(":" + server_name + " " + server_name + " 403 " + client_name + " #" + channel_name + " :No such channel")

#define P_ERR_NOTONCHANNEL(server_name, client_name, channel_name) \
(":" + server_name + " 442 " + client_name + " #" + channel_name + " :You're not on that channel")
// -----

// ----- private msg
#define RPL_PRIVMSG(sender, recipient, message) \
(":" + sender + " PRIVMSG " + recipient + " " + message + "\n")

#define ERR_NOSUCKNICK(servername, nickname) \
(":" + servername + " 401 * " + nickname + " :No such nick/channel\n")

#define ERR_NOTEXTTOSEND(servername) \
("No text to send\n")

// NOTE added a '_' due to redefinition from TOPIC.hpp
#define ERR_NOSUCHCHANNEL_(servername, channel_name) \
(":" + servername + " 403 * " + channel_name + " :No such channel\n")

#define ERR_CANNOTSENDTOCHAN(servername, channel_name) \
(":" + servername + " 404 * " + channel_name + " :Cannot send to channel\n")
// -----

// ----- topic
#define ERR_NEEDMOREPARAMS(servername, nickname) \
(":" + servername + " 461 " + nickname + " TOPIC :Not enough parameters\n")

#define ERR_NOTONCHANNEL(servername, nickname, channel) \
(":" + servername + " 442 " + nickname + " #" + channel + " :You're not on that channel\n")

#define ERR_NOSUCHCHANNEL(servername, nickname, channel) \
(servername + " 403 " + nickname + " #" + channel + " :No such channel\n")

#define ERR_CHANOPRIVSNEEDED(servername, nickname, channel) \
(servername + " 482 " + nickname + " #" + channel + " :You're not channel operator\n")

#define RPL_TOPIC(servername, nickname, channel, topic) \
(":" + servername + " 332 " + nickname + " #" + channel + " :" + topic + "\n")

#define RPL_NOTOPIC(servername, nickname, channel) \
(":" + servername + " 331 " + nickname  + " #" + channel + " :No topic is set" + "\n")
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