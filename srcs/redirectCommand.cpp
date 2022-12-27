#include "redirectCommand.hpp"

redirectCommand::redirectCommand(/* args */)
{
}

redirectCommand::~redirectCommand()
{
}

string	redirectCommand::redirect(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	string cmd = message.getCmdType();

	/* ******    ADD IF CONDITION FOR YOUR COMMAND AND RETURN <YOUR_COMMAND>::execute() METHOD    ****** */
	if (cmd == "TOPIC")
		return topic.execute(message, globalUserList, globalChannelList);
	else
		; //return ERR_UNKNOWNCOMMAND
	return "";
}
