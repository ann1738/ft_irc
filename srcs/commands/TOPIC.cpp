#include "TOPIC.hpp"

TOPIC::TOPIC()
{
}

TOPIC::~TOPIC()
{
}

string	TOPIC::getTopic() const{
	return m_channel->getTopic();
}

void	TOPIC::setTopic(const string &newTopic){
	m_channel->setTopic(newTopic);
}

bool TOPIC::isUserOnChannel(){
	return m_channel->isUser(m_user);
}

bool TOPIC::isSafeTopicModeOn(){
	return m_channel->getTopicSafe();
}

bool TOPIC::isTopicChangeRequested(){
	return topicChangeRequested;
}

void TOPIC::organizeInfo(command msg){
	m_user = msg.getClient();
	string	parameters = msg.getParameters();
	
	if (parameters.at(parameters.size() - 1) == '\n') parameters.resize(parameters.size() - 1);
	if (parameters.at(parameters.size() - 1) == '\r') parameters.resize(parameters.size() - 1);

	/* remove '#' from input */
	if (parameters.at(0) == '#') parameters.erase(parameters.begin());

	/* store the channel's name and topic */
	size_t endIndex = parameters.find_first_of(' ');
	if (endIndex != string::npos) //there are spaces (meaning that it's a topic change request)
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

void	TOPIC::constructReplyMsg(const command &message, bool isChannel){

	if (m_parsedChannelName.empty())
		m_reply = ERR_NEEDMOREPARAMS(m_user.getServername(), m_user.getNickname(), message.getCmdType());
	/*	 Channel does not exist	*/
	else if (!isChannel)
		m_reply = ERR_NOSUCHCHANNEL(m_user.getServername(), m_parsedChannelName);
	/*	 User is not on the channel	*/
	else if (isTopicChangeRequested() && isUserOnChannel() == false )
		m_reply = ERR_NOTONCHANNEL(m_user.getServername(), m_user.getNickname(), m_channel->getName());
	/*	Channel restricts changing the topic to operators and user is not an operator	*/
	else if (isTopicChangeRequested() && isSafeTopicModeOn() && m_channel->isOperator(m_user) == false)
		m_reply = ERR_CHANOPRIVSNEEDED(m_user.getServername(), m_user.getNickname(), m_channel->getName());
	/*	Uses requests a topic change */
	else if (isTopicChangeRequested())
	{
		setTopic(m_parsedChannelTopic);
		m_reply = RPL_TOPIC(m_user.getServername(), m_user.getNickname(), m_channel->getName(), m_channel->getTopic());
	}
	/* Channel does not have a topic	*/
	else if (m_channel->getTopic().empty())
		m_reply = RPL_NOTOPIC(m_user.getServername(), m_user.getNickname(), m_channel->getName());
	/*	User requests to read the channel topic	*/
	else
		m_reply = RPL_TOPIC(m_user.getServername(), m_user.getNickname(), m_channel->getName(), m_channel->getTopic());
}

bool	TOPIC::isReplyForChannel(){
	return (isTopicChangeRequested() && m_reply == RPL_TOPIC(m_user.getServername(), m_user.getNickname(), m_channel->getName(), m_channel->getTopic()));
}

vector<reply>	TOPIC::execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	organizeInfo(message);

	vector<reply>	r;
	r.push_back(reply());

	vector<channel>::iterator iter = findChannel(m_parsedChannelName, globalChannelList);
	bool	isChannel = (iter != globalChannelList.end());
	
	if (isChannel)
		m_channel = &*iter; 

	constructReplyMsg(message, isChannel);
	
	r[0].setMsg(m_reply);
	(isChannel && isReplyForChannel())? r[0].setUserFds(*iter): r[0].setUserFds(m_user);

	return r;
}
