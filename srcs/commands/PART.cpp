#include "../../includes/commands/PART.hpp"

PART::PART(){}

size_t	PART::getEnd(const string& s, size_t start){
	size_t end = s.find(',', start);

	if (end == (size_t)(-1) || ((s.find_first_of("\r\n", start) != (size_t)(-1)) && end > s.find_first_of("\r\n", start)))
		end = s.find_first_of("\r\n", start);
	return (end);
}

void	PART::parseCmdParameters(const string& parameters){
	for (size_t start = (parameters[0] == '#')? parameters.find_last_of(" :"): 0; start != (size_t)(-1); start = parameters.find(',', start)){
		if (start)
			start++;
		size_t end = this->getEnd(parameters, start);
		this->channel_names.push_back(parameters.substr(start, (end - start)));
		if (!start)
			start++;
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

vector<reply>	PART::doPartAction(user& client, vector<channel> &globalChannelList){
	vector<reply> ret;

	for(size_t i = 0; i < this->channel_names.size(); i++){
		pair<size_t, string> temp  = this->goThroughErrors(client, i, globalChannelList);
		ret.push_back(reply());

		if (!temp.second.size()) {
			client.removeChannel(this->channel_names[i]);
			globalChannelList[temp.first].removeUser(client);
			temp.second = RPL_PART(client.getNickname(), this->channel_names[i]);
			ret[i].setUserFds(globalChannelList[temp.first]);
		}
		ret[i].setUserFds(client);
		ret[i].setMsg(temp.second);

	}
	return (ret);
}

vector<reply>	PART::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	
	this->parseCmdParameters(msg.getParameters());
	return (this->doPartAction(msg.getClient(), globalChannelList));
}

PART::~PART(){}
