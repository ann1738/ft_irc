#include "MODE.hpp"

MODE::MODE()
{
}

MODE::~MODE()
{
}


void	MODE::parseChannelName(){

}

void	MODE::parseModes(){

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
	parseChannelName();
	parseModes();

	if (isChannel(parsedChannelName) == false)
		;	// m_reply = 
	
	changeModes();
}
