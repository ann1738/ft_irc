#include "QUIT.hpp"

QUIT::QUIT(){}

QUIT::~QUIT(){}

string			QUIT::parseQuitMsg(const string &paramaters){
	string	parsedQuitMsg;
	parsedQuitMsg = paramaters.substr(1);
	parsedQuitMsg.resize(parsedQuitMsg.size() - 1);
	
	return parsedQuitMsg;
}

vector<reply>	QUIT::constructReply(const command &msg, const string &parsedQuitMsg, vector<channel> &globalChannelList){
	size_t			replyIndex = 0;
	vector<reply>	replies;

	for (vector<channel>::iterator channelIter = globalChannelList.begin(); channelIter != globalChannelList.end(); channelIter++){
		if (channelIter->isUser(msg.getClient()) == true)
		{
			replies.push_back(reply());
			channelIter->removeUser(msg.getClient());
			replies[replyIndex].setMsg(RPL_QUIT(msg.getClient().getServername(), channelIter->getName(), msg.getClient().getNickname(), parsedQuitMsg));
			replies[replyIndex].setUserFds(*channelIter);
			replyIndex++;
		}
	}

	if (replies.empty())
		replies.push_back(reply());
	
	return replies;
}


vector<reply>    QUIT::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	string	parsedQuitMsg = parseQuitMsg(msg.getParameters());

	return constructReply(msg, parsedQuitMsg, globalChannelList);
}