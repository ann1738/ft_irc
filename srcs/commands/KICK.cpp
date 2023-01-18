#include "KICK.hpp"

KICK::KICK(): kicked_client(""), channel_name(""), reason(""){
	;
}

void			KICK::parseParameters(const string& parameters) {
	size_t space = parameters.find(' ');
	size_t second_space = parameters.find(' ', (space + 1));
	size_t colon = parameters.find(':', second_space) + 1;

	channel_name = parameters.substr(1, (space++ - 1));
	kicked_client = parameters.substr(space , (second_space - space));
	reason = (colon == parameters.size())? "": \
	parameters.substr(colon, (parameters.size() - colon - 1));

	// cout << "|channel_name => " << channel_name << "|" << endl;
	// cout << "|kicked_client => " << kicked_client << "|" << endl;
	// cout << "|reason => " << reason << "|" << endl;
}

size_t			KICK::findUser(const vector<user> &globalUserList) const {
	for (size_t i = 0; i < globalUserList.size(); i++) {
		if (globalUserList[i].getNickname() == this->kicked_client)
			return (i);
	}
	return (string::npos);
}
size_t			KICK::findChannel(const vector<channel> &globalChannelList) const {
	for (size_t i = 0; i < globalChannelList.size(); i++) {
		if (globalChannelList[i].getName() == this->channel_name)
			return (i);
	}
	return (string::npos);
}

string			KICK::goThroughErrors(const user& client, const vector<user> &globalUserList, const vector<channel> &globalChannelList) const {
	size_t channel_index = this->findChannel(globalChannelList);
	size_t user_index = this->findUser(globalUserList);

	if (channel_index == string::npos)
		return (ERR_NOSUCHCHANNEL(client.getServername(), channel_name));
	if (user_index == string::npos)
		return (ERR_NOSUCHNICK(client.getServername(), kicked_client));
	if (!globalChannelList[channel_index].isUser(client))
		return (ERR_NOTONCHANNEL(client.getServername(), client.getNickname(), channel_name));
	if (!globalChannelList[channel_index].isOperator(client))
		return (ERR_CHANOPRIVSNEEDED(client.getServername(), client.getNickname(), channel_name));

	return ("");
}

vector<reply>	KICK::doKickAction(user& client, vector<user> &globalUserList, vector<channel> &globalChannelList) {
	vector<reply> r;
	string msg = goThroughErrors(client, globalUserList, globalChannelList);

	r.push_back(reply());
	if (msg.empty()) {
		globalUserList[findUser(globalUserList)].removeChannel(channel_name);
		globalChannelList[findChannel(globalChannelList)].removeUser((globalUserList[findUser(globalUserList)]));
		r[0].setMsg(RPL_KICK(client.getNickname(), kicked_client, channel_name, reason));
		r[0].setUserFds(globalChannelList[findChannel(globalChannelList)]);
		r.push_back(reply());
		r[1].setMsg(RPL_KICKED(client.getServername(), client.getNickname(), channel_name, reason) + RPL_PART(kicked_client, channel_name, reason));
		r[1].setUserFds(globalUserList[findUser(globalUserList)]);
	}
	else {
		r[0].setMsg(msg);
		r[0].setUserFds(client);
	}
	return (r);
}

vector<reply>	KICK::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	this->parseParameters(msg.getParameters());
	return (this->doKickAction(msg.getClient(), globalUserList, globalChannelList));
}

KICK::~KICK(){
	;
}
