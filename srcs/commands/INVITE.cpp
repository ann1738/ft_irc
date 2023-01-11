#include "INVITE.hpp"

INVITE::INVITE(): invited_client(""), channel_name("") {
	;
}

void	INVITE::parseParameters(const string& parameters){
	size_t space = parameters.find(' ');
	invited_client = parameters.substr(0, space);
	space = (parameters.find('#'))? parameters.find('#'): space;
	channel_name = parameters.substr(++space);
}

size_t	INVITE::findUser(vector<user> &globalUserList){
	for (size_t i = 0; i < globalUserList.size(); i++) {
		if (globalUserList[i].getNickname() == this->invited_client)
			return (i);
	}
	return (string::npos);
}

size_t	INVITE::findChannel(vector<channel> &globalChannelList){
	for (size_t i = 0; i < globalChannelList.size(); i++) {
		if (globalChannelList[i].getName() == this->channel_name)
			return (i);
	}
	return (string::npos);
}

string	INVITE::goThroughErrors(user& client, vector<user> &globalUserList, vector<channel> &globalChannelList) {
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

vector<reply>	INVITE::doInviteAction(user& client, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	vector<reply> r;
	string msg = goThroughErrors(client, globalUserList, globalChannelList);

	r.push_back(reply());
	r[0].setMsg((msg == "")? RPL_INVITING(client.getNickname(), channel_name): msg);
	r[0].setUserFds(client);
	return (r);
}

vector<reply>	INVITE::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	this->parseParameters(msg.getParameters());
	return (this->doInviteAction(msg.getClient(), globalUserList, globalChannelList));
}

INVITE::~INVITE() {
	;
}