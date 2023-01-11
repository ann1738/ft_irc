#include "INVITE.hpp"

INVITE::INVITE(): invited_client(""), channel_name("") {
	;
}

void	INVITE::parseParameters(const string& parameters){
	size_t space = parameters.find(' ');
	invited_client = parameters.substr(0, space);
	space = ((parameters.find('#') != string::npos)? parameters.find('#'): parameters.find(' ')) + 1;
	channel_name = parameters.substr(space, (parameters.size() - space - 1));
}

size_t	INVITE::findUser(const vector<user> &globalUserList) const{
	for (size_t i = 0; i < globalUserList.size(); i++) {
		if (globalUserList[i].getNickname() == this->invited_client)
			return (i);
	}
	return (string::npos);
}

size_t	INVITE::findChannel(const vector<channel> &globalChannelList) const{
	for (size_t i = 0; i < globalChannelList.size(); i++) {
		if (globalChannelList[i].getName() == this->channel_name)
			return (i);
	}
	return (string::npos);
}

string	INVITE::goThroughErrors(const user& client, const vector<user> &globalUserList, const vector<channel> &globalChannelList) const{
	size_t channel_index = this->findChannel(globalChannelList);
	size_t user_index = this->findUser(globalUserList);

	if (user_index == string::npos)
		return (ERR_NOSUCHNICK(client.getServername(), invited_client));
	if (channel_index == string::npos)
		return (ERR_NOSUCHCHANNEL(client.getServername(), channel_name));
	if (!globalChannelList[channel_index].isUser(client))
		return (ERR_NOTONCHANNEL(client.getServername(), client.getNickname(), channel_name));
	if (!globalChannelList[channel_index].isOperator(client))
		return (ERR_CHANOPRIVSNEEDED(client.getServername(), client.getNickname(), channel_name));
	if (globalChannelList[channel_index].isUser(globalUserList[user_index]))
		return (ERR_USERONCHANNEL(client.getServername(), invited_client, channel_name));
	return ("");
}

vector<reply>	INVITE::doInviteAction(const user& client, const vector<user> &globalUserList, vector<channel> &globalChannelList) {
	vector<reply> r;
	string msg = goThroughErrors(client, globalUserList, globalChannelList);

	r.push_back(reply());
	if (msg.empty()) {
		globalChannelList[findChannel(globalChannelList)].addInvitedUser(globalUserList[findUser(globalUserList)]);
		r[0].setMsg(RPL_INVITING(client.getServername(), invited_client, channel_name));
		r[0].setUserFds(client);
		r.push_back(reply());
		r[1].setMsg(RPL_INVITED(client.getServername(), client.getNickname(), channel_name));
		r[1].setUserFds(globalUserList[findUser(globalUserList)]);
	}
	else {
		r[0].setMsg(msg);
		r[0].setUserFds(client);
	}
	return (r);
}

vector<reply>	INVITE::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	this->parseParameters(msg.getParameters());
	return (this->doInviteAction(msg.getClient(), globalUserList, globalChannelList));
}

INVITE::~INVITE() {
	;
}