#include "JOIN.hpp"

JOIN::JOIN(){}

size_t	JOIN::getEnd(const string& s, size_t start = 0) {
	size_t end = s.find(',', start);

	if (end == (size_t)(-1) || ((s.find(' ', start) != (size_t)(-1)) && end > s.find(' ', start)))
		end = s.find(' ', start);
	
	if (end == (size_t)(-1) || ((s.find("\r\n", start) != (size_t)(-1)) && end > s.find("\r\n", start)))
		end = s.find("\r\n", start);
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
	for (vector<string>::const_iterator it = this->channel_names.cbegin(); it != this->channel_names.cend(); it++)
		cout << "Channel: " << *it << endl;

	for (vector<string>::const_iterator it = this->keys.cbegin(); it != this->keys.cend(); it++)
		cout << "Key: " << *it << endl;
}

bool	JOIN::inviteError(user& client, vector<channel> &globalChannelList, size_t i) const{
	if (globalChannelList[i].getInviteOnly()) {
		if (!globalChannelList[i].isInvitedUser(client))
			return (true);
	}
	return (false);
}

bool	JOIN::keyError(vector<channel> &globalChannelList, size_t i, size_t position) const{
	if (globalChannelList[i].getKeyEnabled()) {
		if (!this->keys.size() || this->keys[position] != globalChannelList[i].getKey())
			return (true);
	}
	return (false);
}

bool	JOIN::channelLimitError(vector<channel> &globalChannelList, size_t i) const{
	if (globalChannelList[i].getUserCountLimited()) {
		if (globalChannelList[i].getUserCount() == globalChannelList[i].getUserCountLimit())
			return (true);
	}
	return (false);
}


pair<size_t, string>	JOIN::goThroughErrors(user& client, size_t position, vector<channel> &globalChannelList){
	size_t	i = 0;
	bool	chan_not_found = true;
	
	for (; !globalChannelList.empty() && i < globalChannelList.size(); i++){
		if (globalChannelList[i].getName() == this->channel_names[position]) {
			chan_not_found = false;

			if (this->inviteError(client, globalChannelList, i))
				return (make_pair(i, ERR_INVITEONLYCHAN(client.getServername(), client.getNickname(), this->channel_names[position])));//ERR_INVITEONLYCHAN 473
			
			if (this->keyError(globalChannelList, i, position))
				return (make_pair(i, ERR_BADCHANNELKEY(client.getServername(), client.getNickname(), this->channel_names[position])));// ERR_BADCHANNELKEY 475

			if (this->channelLimitError(globalChannelList, i))
				return (make_pair(i, ERR_CHANNELISFULL(client.getServername(), client.getNickname(), this->channel_names[position])));// ERR_CHANNELISFULL 471

			// ?? channel mask ERR_BADCHANMASK 476 ??
			// user limit ERR_TOOMANYCHANNELS 405

			break ;
		}
	}
	if (chan_not_found)
		globalChannelList.push_back(channel(this->channel_names[position], ""));

	return (pair<size_t, string>(i, ""));
}

string	JOIN::doJoinAction(user& client, vector<channel> &globalChannelList){
	string ret;
	for(size_t i = 0; i < this->channel_names.size(); i++){
		pair<size_t, string> temp  = this->goThroughErrors(client, i, globalChannelList);
		
		if (!temp.second.size()) {
			client.addChannel(this->channel_names[i]);
			globalChannelList[temp.first].addUser(client);
			temp.second = JOIN_CORRECT(client.getNickname(), this->channel_names[i]);
			/* --------- making the channel invite only to test error sending --------- */
			// globalChannelList[temp.first].setInviteOnly(true);
		}
		
		ret += temp.second;
		ret.push_back('\n');
	}
	return (ret);
}

string	JOIN::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;

	this->parseCmdParameters(msg.getParameters());
	return (this->doJoinAction(msg.getClient(), globalChannelList));
}

JOIN::~JOIN(){}
