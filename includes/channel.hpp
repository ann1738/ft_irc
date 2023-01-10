#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "user.hpp"

using namespace std;

#define MAX_SIZE_T static_cast<size_t>(-1)

class channel
{
private:
	string	m_name;
	string	m_topic;

	string	m_key;
	int		m_userCountLimit;

	bool	isInviteOnly;        //i
	bool	isModerated;         //m
	bool	isNoExternalMsg;     //n
	bool	isPrivate;           //p
	bool	isSecret;            //s
	bool	isTopicSafe;         //t
	bool	isKeyEnabled;        //k
	bool	isUserCountLimited;  //l

	vector<const user *> users;
	vector<const user *> operators;
	vector<const user *> invitedUsers;
	vector<const user *> voicedUsers;

	size_t	findUser(const vector<const user *> &userList, const user &User) const;

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

	string	getChannelModes() const;

	const vector<const user *>	getUsers() const;
	const vector<const user *>	getOperators() const;
	const vector<const user *>	getInvitedUsers() const;
	const vector<const user *>	getVoicedUsers() const;

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

	void	addUser(const user *User);
	void	removeUser(const user &User);
	bool	isUser(const user &User) const;

	void	addOperator(const user *Op);
	void	removeOperator(const user &Op);
	bool	isOperator(const user &Op) const;
	
	void	addVoicedUser(const user *User);
	void	removeVoicedUser(const user &User);
	bool	isVoicedUser(const user &User) const;
	
	void	addInvitedUser(const user *User);
	void	removeInvitedUser(const user &User);
	bool	isInvitedUser(const user &User) const;
};

#endif 
