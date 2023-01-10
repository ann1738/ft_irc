#include "channel.hpp"

channel::channel(): 
m_name(""), m_topic(""), m_key(""), m_userCountLimit(0), isInviteOnly(false), 
isModerated(false), isNoExternalMsg(false), isPrivate(false), isSecret(false), isTopicSafe(false), 
isKeyEnabled(false), isUserCountLimited(false)
{
}

channel::~channel()
{
}

channel::channel(string name, string topic): m_name(name), m_topic(topic), m_key(""), m_userCountLimit(0), isInviteOnly(false), 
isModerated(false), isNoExternalMsg(false), isPrivate(false), isSecret(false), isTopicSafe(false), 
isKeyEnabled(false), isUserCountLimited(false)
{
}
/* helper function */
size_t	channel::findUser(const vector<const user *> &userList, const user &User) const{
	size_t	index = 0;
	for (vector<const user *>::const_iterator it = userList.begin(); it != userList.end(); it++, index++)
	{
		if ((*it)->getNickname() == User.getNickname())
			return index;
	}
	return -1;
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
	return users.size();
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

string	channel::getChannelModes() const{
	string	modes;
	if (this->getPrivate())
		modes += "p"; 
	if (this->getSecret())
		modes += "s"; 
	if (this->getInviteOnly())
		modes += "i";
	if (this->getTopicSafe())
		modes += "t";
	if (this->getNoExternalMsg())
		modes += "n";
	if (this->getModerated())
		modes += "m"; 
	if (this->getUserCountLimited())
		modes += "l"; 
	if (this->getKeyEnabled())
		modes += "k"; 
	return modes;
}

/* vectors */
const vector<const user *>	channel::getUsers() const{
	return users;
}

const vector<const user *>	channel::getOperators() const{
	return operators;
}

const vector<const user *>	channel::getInvitedUsers() const{
	return invitedUsers;
}

const vector<const user *>	channel::getVoicedUsers() const{
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
void	channel::addUser(user const *User){
	users.push_back(User);
	if (users.size() == 1)    //make the first the channel user an operator 
		addOperator(User);
}

void	channel::removeUser(const user &User){
	users.erase(users.begin() + findUser(users, User));
	if (isOperator(User))
		removeOperator(User);
	if (isInvitedUser(User))
		removeInvitedUser(User);
	if (isVoicedUser(User))
		removeVoicedUser(User);
}

bool	channel::isUser(const user &User) const{
	if (users.empty() || findUser(users, User) == MAX_SIZE_T)
		return false;
	return true;
}


void	channel::addOperator(const user *Op){
	operators.push_back(Op);
}

void	channel::removeOperator(const user &Op){
	operators.erase(operators.begin() + findUser(operators, Op));
}

bool	channel::isOperator(const user &Op) const{
	if (operators.empty() || findUser(operators, Op) == MAX_SIZE_T)
		return false;
	return true;
}

void	channel::addVoicedUser(const user *User){
	voicedUsers.push_back(User);
}

void	channel::removeVoicedUser(const user &User){
	voicedUsers.erase(voicedUsers.begin() + findUser(voicedUsers, User));
}

bool	channel::isVoicedUser(const user &User) const{
	if (voicedUsers.empty() || findUser(voicedUsers, User) == MAX_SIZE_T)
		return false;
	return true;

}

void	channel::addInvitedUser(const user *User){
	invitedUsers.push_back(User);
}

void	channel::removeInvitedUser(const user &User){
	invitedUsers.erase(invitedUsers.begin() + findUser(invitedUsers, User));
}

bool	channel::isInvitedUser(const user &User) const{
	if (invitedUsers.empty() || findUser(invitedUsers, User) == MAX_SIZE_T)
		return false;
	return true;
}
