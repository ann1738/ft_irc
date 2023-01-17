#include "JOIN.hpp"

JOIN::JOIN(){}

size_t	JOIN::getEnd(const string& s, size_t start = 0) {
	size_t end = s.find(',', start);

	if (end == (size_t)(-1) || ((s.find(' ', start) != (size_t)(-1)) && end > s.find(' ', start)))
		end = s.find(' ', start);
	
	if (end == (size_t)(-1) || ((s.find_first_of("\r\n", start) != (size_t)(-1)) && end > s.find_first_of("\r\n", start)))
		end = s.find_first_of("\r\n", start);
	return (end);
}

void	JOIN::saveChannelNames(const string& parameters) {
	for (size_t start = parameters.find('#'); start != (size_t)(-1); start = parameters.find('#', start)){
		size_t end = this->getEnd(parameters, start);
		start++;
		this->channel_names.push_back(parameters.substr(start, (end - start)));
	}
}

void	JOIN::saveKeys(const string& parameters){
	for (size_t start = parameters.find(' '); start != (size_t)(-1); start = parameters.find(',', start)){
		start++;
		size_t end = this->getEnd(parameters, start);
		this->keys.push_back(parameters.substr(start, (end - start)));
	}
}

void	JOIN::parseCmdParameters(const string& parameters){
	this->saveChannelNames(parameters);
	this->saveKeys(parameters);

	this->testingPars();
}

void	JOIN::testingPars() const {
	for (vector<string>::const_iterator it = this->channel_names.begin(); it != this->channel_names.end(); it++)
		cout << "Channel: " << *it << endl;

	for (vector<string>::const_iterator it = this->keys.begin(); it != this->keys.end(); it++)
		cout << "Key: " << *it << endl;
}

bool	JOIN::isInviteError(user& client, vector<channel> &globalChannelList, size_t i) const{
	if (globalChannelList[i].getInviteOnly()) {
		if (!globalChannelList[i].isInvitedUser(client))
			return (true);
	}
	return (false);
}

bool	JOIN::isKeyError(vector<channel> &globalChannelList, size_t i, size_t position) const{
	if (globalChannelList[i].getKeyEnabled()) {
		if (!this->keys.size() || this->keys[position] != globalChannelList[i].getKey())
			return (true);
	}
	return (false);
}

bool	JOIN::isChannelLimitError(vector<channel> &globalChannelList, size_t i) const{
	if (globalChannelList[i].getUserCountLimited()) {
		if (globalChannelList[i].getUserCount() == globalChannelList[i].getUserCountLimit())
			return (true);
	}
	return (false);
}

bool	JOIN::isUserLimitError(user& client) const{
	if (client.getChannelSize() == USER_CHANNEL_LIMIT)
		return (true);
	return (false);
}


pair<size_t, string>	JOIN::goThroughErrors(user& client, size_t position, vector<channel> &globalChannelList){
	size_t	i = 0;
	bool	chan_not_found = true;
	
	for (; !globalChannelList.empty() && i < globalChannelList.size(); i++){
		if (globalChannelList[i].getName() == this->channel_names[position]) {
			chan_not_found = false;

			if (this->isInviteError(client, globalChannelList, i))
				return (make_pair(i, ERR_INVITEONLYCHAN(client.getServername(), client.getNickname(), this->channel_names[position])));//ERR_INVITEONLYCHAN 473
			
			if (this->isKeyError(globalChannelList, i, position))
				return (make_pair(i, ERR_BADCHANNELKEY(client.getServername(), client.getNickname(), this->channel_names[position])));// ERR_BADCHANNELKEY 475

			if (this->isChannelLimitError(globalChannelList, i))
				return (make_pair(i, ERR_CHANNELISFULL(client.getServername(), client.getNickname(), this->channel_names[position])));// ERR_CHANNELISFULL 471

			if (this->isUserLimitError(client))
				return (make_pair(i, ERR_TOOMANYCHANNELS(client.getServername(), client.getNickname())));

			break ;
		}
	}
	if (chan_not_found)
		globalChannelList.push_back(channel(this->channel_names[position], ""));

	return (make_pair(i, ""));
}

template<class recipient>
void	JOIN::saveReplyMsgAndFds(reply& rep, const recipient& fds, const string& msg){
	rep.setUserFds(fds);
	rep.setMsg(msg);
}

void	JOIN::connectUserAndChannel(user& client, channel& chan){
	client.addChannel(chan.getName());
	chan.addUser(client);
}

vector<reply>	JOIN::doJoinAction(user& client, vector<channel> &globalChannelList){
	vector<reply> ret;
	int ret_i = 0;
	for(size_t i = 0; i < this->channel_names.size(); i++, ret_i++){
		pair<size_t, string> temp  = this->goThroughErrors(client, i, globalChannelList);
		ret.push_back(reply());
	
		if (!temp.second.size()) {
			this->connectUserAndChannel(client, globalChannelList[temp.first]);
			this->saveReplyMsgAndFds(ret[ret_i], globalChannelList[temp.first], RPL_JOIN(client.getNickname(), this->channel_names[i]));
			if (!globalChannelList[temp.first].getTopic().empty()) {
				ret.push_back(reply());
				this->saveReplyMsgAndFds(ret[++ret_i], client, RPL_TOPIC(client.getServername(), client.getNickname(), channel_names[i], globalChannelList[temp.first].getTopic()));
			}
		}
		else 
			this->saveReplyMsgAndFds(ret[ret_i], client, RPL_KICK(client.getNickname(), client.getNickname(), channel_names[i], "") + temp.second);
	}
	return (ret);
}

vector<reply>	JOIN::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;

	this->parseCmdParameters(msg.getParameters());
	return (this->doJoinAction(msg.getClient(), globalChannelList));
}

JOIN::~JOIN(){}
