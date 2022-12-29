#include "channel.hpp"

channel::channel(): 
m_name(""), m_topic(""), m_key(""), m_userCountLimit(0), m_userCount(0), isInviteOnly(false), 
isModerated(false), isNoExternalMsg(false), isPrivate(false), isSecret(false), isTopicSafe(false), 
isKeyEnabled(false), isUserCountLimited(false)
{
}

channel::~channel()
{
}

channel::channel(string name, string topic): m_name(name), m_topic(topic), m_key(""), m_userCountLimit(0), m_userCount(0), isInviteOnly(false), 
isModerated(false), isNoExternalMsg(false), isPrivate(false), isSecret(false), isTopicSafe(false), 
isKeyEnabled(false), isUserCountLimited(false)
{
}
/* helper function */
vector<user>::const_iterator	channel::findUser(vector<user> const &userList, user User) const{
	for (vector<user>::const_iterator it = userList.begin(); it != userList.end(); it++)
	{
		if (it->getNickname() == User.getNickname())
			return it;
	}
	return userList.end();
}

/* getters */
string	channel::getName() const{
	return m_name;
}

string	channel::getTopic() const{
	return m_topic;
}

string	channel::getKey() const{
	return m_key;
}

int		channel::getUserCountLimit() const{
	return m_userCountLimit;
}

int		channel::getUserCount() const{
	return m_userCount;
}

/* modes */
bool	channel::getInviteOnly() const{
	return isInviteOnly;
}

bool	channel::getModerated() const{
	return isModerated;
}

bool	channel::getNoExternalMsg() const{
	return isNoExternalMsg;
}

bool	channel::getPrivate() const{
	return isPrivate;
}

bool	channel::getSecret() const{
	return isSecret;
}

bool	channel::getTopicSafe() const{
	return isTopicSafe;
}

bool	channel::getKeyEnabled() const{
	return isKeyEnabled;
}

bool	channel::getUserCountLimited() const{
	return isUserCountLimited;
}

/* vectors */
const vector<user>	channel::getUsers() const{
	return users;
}

const vector<user>	channel::getOperators() const{
	return operators;
}

const vector<user>	channel::getInvitedUsers() const{
	return invitedUsers;
}

const vector<user>	channel::getVoicedUsers() const{
	return voicedUsers;
}

/* setters*/
void	channel::setName(string name){
	m_name = name;
}
void	channel::setTopic(string topic){
	m_topic = topic;
}


void	channel::setKey(string Key){
	m_key = Key;
}

void	channel::setUserCountLimit(int limit){
	m_userCountLimit = limit;
}

void	channel::setUserCount(int count){
	m_userCount = count;
}

/* modes */
void	channel::setInviteOnly(bool mode){
	isInviteOnly = mode;
}

void	channel::setModerated(bool mode){
	isModerated = mode;
}

void	channel::setNoExternalMsg(bool mode){
	isNoExternalMsg = mode;
}

void	channel::setPrivate(bool mode){
	isPrivate = mode;
}

void	channel::setSecret(bool mode){
	isSecret = mode;
}

void	channel::setTopicSafe(bool mode){
	isTopicSafe = mode;
}

void	channel::setKeyEnabled(bool mode){
	isKeyEnabled = mode;
}

void	channel::setUserCountLimited(bool mode){
	isUserCountLimited = mode;
}

/* vectors */
void	channel::addUser(user const &User){
	users.push_back(User);
}

void	channel::removeUser(user const &User){
	users.erase(findUser(users, User));
	if (isOperator(User))
		removeOperator(User);
	if (isInvitedUser(User))
		removeInvitedUser(User);
	if (isVoicedUser(User))
		removeVoicedUser(User);
}

bool	channel::isUser(user const &User) const{
	if (users.empty() || findUser(users, User) == users.end())
		return false;
	return true;
}


void	channel::addOperator(user const &Op){
	operators.push_back(Op);
}

void	channel::removeOperator(user const &Op){
	operators.erase(findUser(operators, Op));
}

bool	channel::isOperator(user const &Op) const{
	if (operators.empty() || findUser(operators, Op) == operators.end())
		return false;
	return true;
}

void	channel::addVoicedUser(user const &User){
	voicedUsers.push_back(User);
}

void	channel::removeVoicedUser(user const &User){
	voicedUsers.erase(findUser(voicedUsers, User));
}

bool	channel::isVoicedUser(user const &User) const{
	if (voicedUsers.empty() || findUser(voicedUsers, User) == voicedUsers.end())
		return false;
	return true;

}

void	channel::addInvitedUser(user const &User){
	invitedUsers.push_back(User);
}

void	channel::removeInvitedUser(user const &User){
	invitedUsers.erase(findUser(invitedUsers, User));
}

bool	channel::isInvitedUser(user const &User) const{
	if (invitedUsers.empty() || findUser(invitedUsers, User) == invitedUsers.end())
		return false;
	return true;
}
