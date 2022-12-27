#ifndef TOPIC_HPP
#define TOPIC_HPP

#include <string>
#include "channel.hpp"
#include "command.hpp"

using namespace std;

// #define RPL_NOTOPIC 331
// #define RPL_TOPIC 332
// #define TOPIC_SUCCESS(nickname, channel, topic) nickname + " changed the topic of " + channel + "to: " + topic

// #define RPL_NOTOPIC(servername, nickname, channel) (":" servername " 331 " nickname " " + channel + " :No topic is set" + "\n")
// #define RPL_TOPIC(servername, nickname, channel, topic) (":" + servername + " 332 " + nickname + " " + channel + " :" + topic + "\n")

// no topic set is handled by irssi

/***
 * 
 * TOPIC #channel :New channel topic  ----- sets a new channel topic
 * 
 * TOPIC #channel                     ----- sets a new channel topic
 * 
 ***/

string rpl_topic(string servername, string nickname, string channel, string topic)
{
	string msg(""); 
	msg = msg + ":" + servername + " 332 " + nickname + " " + channel + " :" + topic + "\n";
	return(msg);
}


class TOPIC
{
private:
	user	m_user;
	channel	m_channel;

	

	void	setTopic(string newTopic);
	string	getTopic();
	bool	checkIfTopicSafe();

public:
	TOPIC();
	~TOPIC();

	string	execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
	void	doTopic(user User, channel Channel, string newTopic, int fd/*, <command_struct> Command*/);
};

TOPIC::TOPIC()
{
}

TOPIC::~TOPIC()
{
}

void		TOPIC::setTopic(string newTopic){
	// if (checkIfTopicSafe() == true && m_channel.isOperator(m_user) == false)
	// {
	// 	// return appropriate error
	// }

	m_channel.setTopic(newTopic);
}

std::string	TOPIC::getTopic(){
	return m_channel.getTopic();
}

bool		TOPIC::checkIfTopicSafe(){
	return m_channel.getTopicSafe();
}

#define ERR_NICKNAMEINUSE(servername, nick) (":" + servername + " 433 * " + nick + " :Nickname is already in use\n")

#define RPL_TOPIC(server, client, channel, topic) (": " server " 332 " client " " channel " :" topic)

string		TOPIC::execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)message;
	(void)globalUserList;
	(void)globalChannelList;
	// setTopic(newTopic);
	// string msg("");
	// msg = msg + ":" + "webarebears" + " 332 " + User.getNickname() + " " + Channel.getName() + " :" + newTopic + "\n";
	// send(fd, msg.c_str(), msg.size(), 0);
	// std::cout << msg << std::endl;
	string msg = rpl_topic("fdsjn", globalUserList[0].getNickname(), "wee", message.getParameters().substr(6));
	std::cout << msg << std::endl;
	return msg;
}


#endif