#ifndef CHANNEL_HPP
#define CHANNEL_HPP

using namespace std;

#include <string>
#include <vector>
#include <algorithm>
#include "user.hpp"

class channel
{
private:
	string	m_name;
	string	m_topic;

	string	m_key;               //empty if channel is not key protected (+k)
	int		m_userCountLimit;    //0 if channel doesn't limit number of users (+l)
	int		m_userCount;         //counts the number of users

	bool	isInviteOnly;        //i
	bool	isModerated;         //m
	bool	isNoExternalMsg;     //n
	bool	isPrivate;           //p
	bool	isSecret;            //s
	bool	isTopicSafe;         //t
	bool	isKeyEnabled;        //k
	bool	isUserCountLimited;  //l

	vector<user> users;
	vector<user> operators;
	vector<user> invitedUsers;   //empty if channel is not invite-only (+i)
	vector<user> voicedUsers;    //empty if channel is not moderated (+m)

	vector<user>::iterator	findUser(vector<user> userList, user User);

	/*** modes to debate: b (ban user mask), 
	 *   modes that are currently confusing (because they are for users): v (voiced users), o (operators)
	 * */

	channel();

public:
	channel(string name, string topic);
	~channel();

	/* getters */
	string	getName();
	string	getTopic();

	string	getKey();
	int		getUserCountLimit();
	int		getUserCount();

	bool	getInviteOnly();
	bool	getModerated();
	bool	getNoExternalMsg();
	bool	getPrivate();
	bool	getSecret();
	bool	getTopicSafe();
	bool	getKeyEnabled();
	bool	getUserCountLimited();

	vector<user>	getUsers();
	vector<user>	getOperators();
	vector<user>	getInvitedUsers();
	vector<user>	getVoicedUsers();

	/* setters */
	void	setName(string name);
	void	setTopic(string topic);

	void	setKey(string Key);
	void	setUserCountLimit(int limit);
	void	setUserCount(int count);

	void	setInviteOnly(bool mode);
	void	setModerated(bool mode);
	void	setNoExternalMsg(bool mode);
	void	setPrivate(bool mode);
	void	setSecret(bool mode);
	void	setTopicSafe(bool mode);
	void	setKeyEnabled(bool mode);
	void	setUserCountLimited(bool mode);

	void	addUser(user User);
	void	removeUser(user User);
	bool	isUser(user User);

	void	addOperator(user Op);
	void	removeOperator(user Op);
	bool	isOperator(user Op);
	
	void	addVoicedUser(user User);
	void	removeVoicedUser(user User);
	bool	isVoicedUser(user User);
	
	void	addInvitedUser(user User);
	void	removeInvitedUser(user User);
	bool	isInvitedUser(user User);
};

#endif 
