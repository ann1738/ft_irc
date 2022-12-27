#ifndef REDIRECTCOMMAND_HPP
#define REDIRECTCOMMAND_HPP

#include <string>
#include "command.hpp"
#include "TOPIC.hpp"
#include "NICK.hpp"

using namespace std;

class redirectCommand
{
private:
	/* IRC COMMANDS ARE DECLARED HERE */
	TOPIC topic;
	NICK nick;

public:
	redirectCommand();
	~redirectCommand();

	string	redirect(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList);
};

redirectCommand::redirectCommand(/* args */)
{
}

redirectCommand::~redirectCommand()
{
}


string	redirectCommand::redirect(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	user u1;

	string cmd = message.getCmdType();
	u1.setNickname("user");

	if (cmd == "TOPIC")
		return topic.execute(message, globalUserList, globalChannelList);
	else if (cmd == "NICK")
	{
		string temp =  message.getClient().getNickname();
		nick.doNickCommand(globalUserList, message.getClient().getFd(), (char *)(message.getCmdType() + message.getParameters()).c_str());
		return nick.buildResponse(temp, message.getClient().getNickname());
	}
	else if (cmd == "PING")
		;
	// else
	// 	return string(":weberebears 421 " + message.getClient().getNickname() + " UNKNOWN_COMMAND :Unknown command");
	return "";
}


#endif