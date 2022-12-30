#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "user.hpp"

using namespace std;

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

	vector<user>::iterator	findUser(vector<user> &userList, user User);

	/*** modes to debate: b (ban user mask), 
	 *   modes that are currently confusing (because they are for users): v (voiced users), o (operators)
	 * */


public:
	channel();
	channel(string name, string topic);
	~channel();

	/* getters */
	string	getName() const;
	string	getTopic() const;

	string	getKey() const;
	int		getUserCountLimit() const;
	int		getUserCount() const;

	bool	getInviteOnly() const;
	bool	getModerated() const;
	bool	getNoExternalMsg() const;
	bool	getPrivate() const;
	bool	getSecret() const;
	bool	getTopicSafe() const;
	bool	getKeyEnabled() const;
	bool	getUserCountLimited() const;

	const vector<user>	getUsers() const;
	const vector<user>	getOperators() const;
	const vector<user>	getInvitedUsers() const;
	const vector<user>	getVoicedUsers() const;

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

	void	addUser(user const &User);
	void	removeUser(user &User);
	bool	isUser(user &User);

	void	addOperator(user const &Op);
	void	removeOperator(user const &Op);
	bool	isOperator(user &Op);
	
	void	addVoicedUser(user const &User);
	void	removeVoicedUser(user const &User);
	bool	isVoicedUser(user &User);
	
	void	addInvitedUser(user const &User);
	void	removeInvitedUser(user const &User);
	bool	isInvitedUser(user &User);
};

#endif 
