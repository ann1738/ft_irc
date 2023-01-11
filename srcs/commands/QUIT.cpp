#include "QUIT.hpp"

QUIT::QUIT(){}

QUIT::~QUIT(){}

string			QUIT::parseQuitMsg(const string &paramaters){
	string	parsedQuitMsg;
	parsedQuitMsg = paramaters.substr(1);
	parsedQuitMsg.resize(parsedQuitMsg.size() - 1);
	
	return parsedQuitMsg;
}

vector<reply>	QUIT::constructReply(const command &msg, const string &parsedQuitMsg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	size_t			channelIndex = 0;
	vector<reply>	replies;

	for (vector<channel>::iterator channelIter = globalChannelList.begin(); channelIter != globalChannelList.end(); channelIter++){
		if (channelIter->isUser(msg.getClient()) == true)
		{
			cout << "I am in this channel" << endl;
			replies.push_back(reply());
			channelIter->removeUser(msg.getClient());
			replies[channelIndex].setMsg(RPL_QUIT(channelIter->getName(), msg.getClient().getNickname(), parsedQuitMsg));
			replies[channelIndex].setUserFds(*channelIter);
			channelIndex++;
		}
	}

	if (replies.empty())
		replies.push_back(reply());
	
	return replies;
}


vector<reply>    QUIT::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	string	parsedQuitMsg;

	parsedQuitMsg = parseQuitMsg(msg.getParameters());
	cout << "QUIT MESSAGE IS " << parsedQuitMsg  << endl;

	return constructReply(msg, parsedQuitMsg, globalUserList, globalChannelList);
}