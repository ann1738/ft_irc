#include "../includes/channel.hpp"

channel::channel()
{
}

channel::~channel()
{
}

channel::channel(string name, string topic): m_name(name), m_topic(topic)
{
}

/* getters */
string	channel::getName(){
	return m_name;
}

string	channel::getTopic(){
	return m_topic;
}

string	channel::getKey(){
	return m_key;
}

int		channel::getUserCountLimit(){
	return m_userCountLimit;
}

int		channel::getUserCount(){
	return m_userCount;
}

/* modes */
bool	channel::getInviteOnly(){
	return isInviteOnly;
}

bool	channel::getModerated(){
	return isModerated;
}

bool	channel::getNoExternalMsg(){
	return isNoExternalMsg;
}

bool	channel::getPrivate(){
	return isPrivate;
}

bool	channel::getSecret(){
	return isSecret;
}

bool	channel::getTopicSafe(){
	return isTopicSafe;
}

bool	channel::getKeyEnabled(){
	return isKeyEnabled;
}

bool	channel::getUserCountLimited(){
	return isUserCountLimited;
}

/* vectors */
vector<user>	channel::getUsers(){
	return users;
}

vector<user>	channel::getOperators(){
	return operators;
}

vector<user>	channel::getInvitedUsers(){
	return invitedUsers;
}

vector<user>	channel::getVoicedUsers(){
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
void	channel::addUser(user User){
	users.push_back(User);
}

void	channel::removeUser(user User){
	(void)User;
	// users.erase(find(users.begin(), users.end(), User));
}


void	channel::addOperator(user Op){
	users.push_back(Op);
}

void	channel::removeOperator(user Op){
	(void)Op;
	// users.erase(find(users.begin(), users.end(), Op));
}


void	channel::addVoicedUser(user User){
	users.push_back(User);
}

void	channel::removeVoicedUser(user User){
	(void)User;
	// users.erase(find(users.begin(), users.end(), User));
}


void	channel::addInvitedUser(user User){
	users.push_back(User);
}

void	channel::removeInvitedUser(user User){
	(void)User;
	// users.erase(find(users.begin(), users.end(), User));
}
