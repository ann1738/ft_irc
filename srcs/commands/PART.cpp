#include "../../includes/commands/PART.hpp"

PART::PART(){}

size_t	PART::getEnd(const string& s, size_t start){
	size_t end = s.find(',', start);

	if (end == string::npos || (s.find_first_of(" :", start) != string::npos && end > s.find_first_of(" :", start)))
		end = s.find_first_of(" :", start);
	if (end == string::npos || (s.find_first_of("\r\n", start) != string::npos && end > s.find_first_of("\r\n", start)))
		end = s.find_first_of("\r\n", start);
	return (end);
}

size_t	PART::getStart(const string& s, size_t start){
	size_t new_start = s.find(',', start);

	if (new_start == string::npos || (s.find_first_of(" :", start) != string::npos && new_start > s.find_first_of(" :", start)))
		new_start = s.find_first_of(" :", start);
	if (new_start == string::npos || (s.find_first_of("\r\n", start) != string::npos && new_start > s.find_first_of("\r\n", start)))
		new_start = s.find_first_of("\r\n", start);
	return (new_start);
}

void	PART::parseCmdParameters(const string& parameters){
	for (size_t start = 0; start != string::npos; start = getStart(parameters, start)){
		if (start || parameters[0] == '#')
			start++;
		size_t end = this->getEnd(parameters, start);
		if (parameters.substr(start, (end - start)) == "")
			break ;
		this->channel_names.push_back(parameters.substr(start, (end - start)));
		if (!start)
			start++;
	}
	if (parameters.find(" :") != string::npos) {
		size_t start = parameters.find(" :") + 2;
		this->reason = parameters.substr(start, (parameters.size()- start));
	}
}

pair<size_t, string>	PART::goThroughErrors(user& client, size_t position, vector<channel> &globalChannelList){
	size_t	i = 0;
	bool	chan_not_found = true;
	
	for (; !globalChannelList.empty() && i < globalChannelList.size(); i++){
		if (globalChannelList[i].getName() == this->channel_names[position]) {
			chan_not_found = false;
			if (!globalChannelList[i].isUser(client))
				return (make_pair(i, ERR_NOTONCHANNEL(client.getServername(), client.getNickname(), this->channel_names[position])));
			break ;
		}
	}
	if (chan_not_found)
		return (make_pair(i, ERR_NOSUCHCHANNEL(client.getServername(), this->channel_names[position])));

	return (make_pair(i, ""));
}

void	PART::saveReplyMsgAndFds(reply& rep, const user& fd, const string& msg){
	rep.setUserFds(fd);
	rep.setMsg(msg);
}

void	PART::disconnectUserAndChannel(user& client, channel& chan){
	client.removeChannel(chan.getName());
	chan.removeUser(client);
}

vector<reply>	PART::doPartAction(user& client, vector<channel> &globalChannelList){
	vector<reply> ret;

	for(size_t i = 0; i < this->channel_names.size(); i++){
		pair<size_t, string> temp  = this->goThroughErrors(client, i, globalChannelList);
		ret.push_back(reply());

		if (!temp.second.size()) {
			this->disconnectUserAndChannel(client, globalChannelList[temp.first]);
			temp.second = RPL_PART(client.getNickname(), this->channel_names[i], this->reason);
			ret[i].setUserFds(globalChannelList[temp.first]);
		}
		saveReplyMsgAndFds(ret[i], client, temp.second);

	}
	return (ret);
}

vector<reply>	PART::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	
	this->parseCmdParameters(msg.getParameters());
	return (this->doPartAction(msg.getClient(), globalChannelList));
}

PART::~PART(){}
