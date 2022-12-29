#ifndef TOPIC_HPP
#define TOPIC_HPP

#include <string>
#include "channel.hpp"
#include "command.hpp"

using namespace std;

#define SERVERNAME "WeBareBears"

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

/***
 * 
 * TOPIC #channel :New channel topic  ----- sets a new channel topic
 * 
 * TOPIC #channel                     ----- sets a new channel topic
 * 
 ***/

class TOPIC
{
private:
	user	m_user;
	channel	m_channel;

	string	m_parsedChannelName;
	string	m_parsedChannelTopic;
	bool	topicChangeRequested;
	
	string	m_reply;

	string	getTopic() const;
	void	setTopic(const string &newTopic);

	bool	isUserOnChannel();
	bool	isSafeTopicModeOn();
	bool	isTopicChangeRequested();
	void	organizeInfo(command msg);

	vector<channel>::iterator	findChannel(const string &channelName, vector<channel> &globalChannelList);

public:
	TOPIC();
	~TOPIC();

	string	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};


#endif
