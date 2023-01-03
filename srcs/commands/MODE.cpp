#include "MODE.hpp"

MODE::MODE()
{
}

MODE::~MODE()
{
}


void	MODE::parseChannelName(string &parameters){
	parsedChannelName = parameters.substr(0, parameters.find(' '));
	//erase the channel name
	parameters.erase(0, parameters.find(' ') + 1);
}

void	MODE::parseModes(string &parameters){
	parsedModes = parameters.substr(0, parameters.find(' '));
	
}

bool	MODE::isChannel(const string& channelName){

}

string	MODE::isMode(const string& modes){

}

void	MODE::changeModes(){

}

void	MODE::constructReply(){

}

string	MODE::execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	string parameters = message.getParameters();
	parseChannelName(parameters);
	parseModes(parameters);

	if (isChannel(parsedChannelName) == false)
		;	// m_reply = 
	
	changeModes();
}
