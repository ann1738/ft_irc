#ifndef TOPIC_HPP
#define TOPIC_HPP

#include <string>
#include "channel.hpp"
#include "command.hpp"

using namespace std;

#define SERVERNAME "WeBareBears"

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

	string	m_reply;

	bool	topicChangeRequested;

	string	getTopic() const;
	void	setTopic(const string &newTopic);

	void	organizeInfo(command msg);
	bool	isTopicChangeRequested();
	bool	isSafeTopicModeOn();
	bool	isUserOnChannel();

	vector<channel>::iterator	findChannel(const string &channelName, vector<channel> &globalChannelList);

	void	build_RPL_TOPIC(string servername, string nickname, string channel, string topic);
	void	build_RPL_NOTOPIC(string servername, string nickname, string channel);
	void	build_ERR_NEEDMOREPARAMS(string servername, string nickname);
	void	build_ERR_NOTONCHANNEL(string servername, string nickname, string channel);
	void	build_ERR_NOSUCHCHANNEL(string servername, string nickname, string channel);
	void	build_ERR_CHANOPRIVSNEEDED(string servername, string nickname, string channel);

public:
	TOPIC();
	~TOPIC();

	string	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};


#endif
