#include "TOPIC.hpp"

/* Reply Functions */

#define ERR_NEEDMOREPARAMS(servername, nickname) \
(":" + servername + " 461 " + nickname + " TOPIC :Not enough parameters\n")

#define RPL_TOPIC(servername, nickname, channel, topic) \
(":" + servername + " 332 " + nickname + " #" + channel + " :" + topic + "\n")

void	TOPIC::build_RPL_TOPIC(string servername, string nickname, string channel, string topic){
	// string msg("");
	// msg += msg + ":" + servername + " 332 " + nickname + " " + channel + " :" + topic + "\n";
	// m_reply = msg;
	m_reply = RPL_TOPIC(servername, nickname, channel, topic);
}

void	TOPIC::build_RPL_NOTOPIC(string servername, string nickname, string channel){
	string msg("");
	msg += ":" + servername + " 331 " + nickname  + " " + channel + " :No topic is set" + "\n";
	m_reply = msg;
}


void	TOPIC::build_ERR_NEEDMOREPARAMS(string servername, string nickname){
	// string msg("");
	// msg += ":" + servername + " 461 " + nickname + " TOPIC :Not enough parameters\n";
	m_reply = ERR_NEEDMOREPARAMS(servername, nickname);
}

void	TOPIC::build_ERR_NOTONCHANNEL(string servername, string nickname, string channel){
	string msg("");
	msg += ":" + servername + " 442 " + nickname + " " + channel + " :You're not on that channel\n";
	m_reply = msg;
}

void	TOPIC::build_ERR_NOSUCHCHANNEL(string servername, string nickname, string channel){
	string msg("");
	msg += servername + " 403 " + nickname + " " + channel + " :No such channel\n";
	m_reply = msg;
}

void	TOPIC::build_ERR_CHANOPRIVSNEEDED(string servername, string nickname, string channel){
	string msg("");
	msg += servername + " 482 " + nickname + " " + channel + " :You're not channel operator\n";
	m_reply = msg;
}


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

/*I am writing the following function but I need more details of how the channel comes through from irssi*/

void TOPIC::organizeInfo(command msg){
	m_user = msg.getClient();
	string	parameters = msg.getParameters();

	/* store information */
	size_t endIndex = parameters.find_first_of(' ');
	if (endIndex != string::npos) //there are spaces (essentially this is a request for changing the topic)
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

	// if (m_parsedName[0] == '\r')
	// 	m_parsedName.clear();
}

bool TOPIC::isTopicChangeRequested(){
	return topicChangeRequested;
}

vector<channel>::iterator	TOPIC::findChannel(const string &channelName, vector<channel> &globalChannelList){
	for (vector<channel>::iterator it = globalChannelList.begin(); it != globalChannelList.end(); it++)
	{
		if (it->getName() == channelName)
			return it;
	}
	return globalChannelList.end();
}

bool TOPIC::isSafeTopicModeOn(){
	return m_channel.getTopicSafe();
}

bool TOPIC::isUserOnChannel(){
	return m_channel.isUser(m_user);
}

/*	LOOK INTO THE ORDER OF THE IF CONDITIONS AND MAKE SURE NOTHING PROBLEMATIC COULD OCCUR	*/
string	TOPIC::execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	organizeInfo(message);

	vector<channel>::iterator iter = findChannel(m_parsedChannelName, globalChannelList);
	if (iter != globalChannelList.end())
		m_channel = *iter; //am I copying a new channel or a reference 
	
	/*	 Not enough paramaters	*/
	if (m_parsedChannelName.empty())             
		build_ERR_NEEDMOREPARAMS(SERVERNAME, m_user.getNickname());
	/*	 Channel does not exist	*/
	else if (iter == globalChannelList.end())
		build_ERR_NOSUCHCHANNEL(SERVERNAME, m_user.getNickname(), m_parsedChannelName);	//channel not found error
	/*	 User is not on the channel	*/
	else if (isUserOnChannel() == false)
		build_ERR_NOTONCHANNEL(SERVERNAME, m_user.getNickname(), m_channel.getName());
	/*	Channel restricts changing the topic to operators and user is not an operator	*/
	else if (isTopicChangeRequested() && isSafeTopicModeOn() && m_channel.isOperator(m_user) == false)
		build_ERR_CHANOPRIVSNEEDED(SERVERNAME, m_user.getNickname(), m_channel.getName());
	/*	Uses requests a topic change */
	else if (isTopicChangeRequested())
	{
		setTopic(m_parsedChannelTopic);
		build_RPL_TOPIC(SERVERNAME, m_user.getNickname(), m_channel.getName(), m_channel.getTopic());
	}
	/* Channel does not have a topic	*/
	else if (m_channel.getTopic().empty())
		build_RPL_NOTOPIC(SERVERNAME, m_user.getNickname(), m_channel.getName());
	/*	User requests to read the channel topic	*/
	else
		build_RPL_TOPIC(SERVERNAME, m_user.getNickname(), m_channel.getName(), m_channel.getTopic());
	return m_reply;
}
