#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "reply.hpp"
#include "command.hpp"

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
	channel	*m_channel;

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

	void	constructReplyMsg(const command &message, vector<channel> &globalChannelList, vector<channel>::iterator iter);
	bool	isReplyForChannel();

	vector<channel>::iterator	findChannel(const string &channelName, vector<channel> &globalChannelList);

public:
	TOPIC();
	~TOPIC();

	vector<reply>	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};


#endif
