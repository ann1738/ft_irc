#include "redirectCommand.hpp"

redirectCommand::redirectCommand()
{
}

redirectCommand::~redirectCommand()
{
}

vector<reply>	redirectCommand::redirect(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	string cmd = message.getCmdType();
	vector<reply> ret;
	ret.push_back(reply());

	/* ******    ADD IF CONDITION FOR YOUR COMMAND AND RETURN <YOUR_COMMAND>::execute() METHOD    ****** */
	if (cmd == "TOPIC")
		return topic.execute(message, globalUserList, globalChannelList);
	else if (cmd == "NICK")
		return nick.execute(message, globalUserList, globalChannelList);
	else if (cmd == "JOIN")
		return join.execute(message, globalUserList, globalChannelList);
	else if (cmd == "PART")
		return part.execute(message, globalUserList, globalChannelList);
	else if (cmd == "PRIVMSG")
		return privmsg.execute(message, globalUserList, globalChannelList);
	else
		ret[0].setMsg(""); // ret[0].setMsg(ERR_UNKNOWNCOMMAND);
	ret[0].setUserFds(message.getClient());
	return ret;
}
