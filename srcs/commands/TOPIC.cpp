#include "TOPIC.hpp"

TOPIC::TOPIC()
{
}

TOPIC::~TOPIC()
{
}

string	TOPIC::getTopic() const{
	return m_channel.getTopic();
}

void	TOPIC::setTopic(const string &newTopic){
	m_channel.setTopic(newTopic);
}

bool TOPIC::isUserOnChannel(){
	return m_channel.isUser(m_user);
}

bool TOPIC::isSafeTopicModeOn(){
	return m_channel.getTopicSafe();
}

bool TOPIC::isTopicChangeRequested(){
	return topicChangeRequested;
}

void TOPIC::organizeInfo(command msg){
	m_user = msg.getClient();
	string	parameters = msg.getParameters();
	
	if (parameters[parameters.size() - 1] == '\r') parameters.pop_back();
	if (parameters[parameters.size() - 1] == '\n') parameters.pop_back();

	/* store the channel's name and topic */
	size_t endIndex = parameters.find_first_of(' ');
	if (endIndex != string::npos) //there are spaces (meaning that it is a request for changing the topic)
	{
		topicChangeRequested = true;
		m_parsedChannelName = parameters.substr(0, endIndex);
		m_parsedChannelTopic = parameters.substr(endIndex + 2);
	}
	else
	{
		topicChangeRequested = false;
		m_parsedChannelName = parameters;
		m_parsedChannelTopic = "";
	}
}

vector<channel>::iterator	TOPIC::findChannel(const string &channelName, vector<channel> &globalChannelList){
	for (vector<channel>::iterator it = globalChannelList.begin(); it != globalChannelList.end(); it++)
	{
		if (it->getName() == channelName)
			return it;
	}
	return globalChannelList.end();
}

string	TOPIC::execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	organizeInfo(message);

	vector<channel>::iterator iter = findChannel(m_parsedChannelName, globalChannelList);
	if (iter != globalChannelList.end())
		m_channel = *iter; //am I copying a new channel or a reference 

	/*	 Not enough paramaters	*/
	if (m_parsedChannelName.empty())
		m_reply = ERR_NEEDMOREPARAMS(string(SERVERNAME), m_user.getNickname());
	/*	 Channel does not exist	*/
	else if (iter == globalChannelList.end())
		m_reply = ERR_NOSUCHCHANNEL(string(SERVERNAME), m_user.getNickname(), m_parsedChannelName);
	/*	 User is not on the channel	*/
	else if (isUserOnChannel() == false)
		m_reply = ERR_NOTONCHANNEL(string(SERVERNAME), m_user.getNickname(), m_channel.getName());
	/*	Channel restricts changing the topic to operators and user is not an operator	*/
	else if (isTopicChangeRequested() && isSafeTopicModeOn() && m_channel.isOperator(m_user) == false)
		m_reply = ERR_CHANOPRIVSNEEDED(string(SERVERNAME), m_user.getNickname(), m_channel.getName());
	/*	Uses requests a topic change */
	else if (isTopicChangeRequested())
	{
		setTopic(m_parsedChannelTopic);
		m_reply = RPL_TOPIC(string(SERVERNAME), m_user.getNickname(), m_channel.getName(), m_channel.getTopic());
	}
	/* Channel does not have a topic	*/
	else if (m_channel.getTopic().empty())
		m_reply = RPL_NOTOPIC(string(SERVERNAME), m_user.getNickname(), m_channel.getName());
	/*	User requests to read the channel topic	*/
	else
		m_reply = RPL_TOPIC(string(SERVERNAME), m_user.getNickname(), m_channel.getName(), m_channel.getTopic());
	return m_reply;
}
