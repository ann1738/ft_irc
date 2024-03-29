#ifndef PART_HPP
#define PART_HPP

#include "reply.hpp"
#include "command.hpp"

class PART {

private:
	vector<string>	channel_names;
	string			reason;

	/* -------------------- parsing -------------------- */
	size_t	getNextIndex(const string& s, size_t start);
	void	parseCmdParameters(const string& parameters);
	/* ------------------------------------------------- */

	/* ------------------- executing ------------------- */
	pair<size_t, string>	goThroughErrors(user& client, size_t position, vector<channel> &globalChannelList);
	
	void	saveReplyMsgAndFds(reply& rep, const user& fds, const string& msg);
	void	disconnectUserAndChannel(user& client, channel& chan);

	vector<reply>			doPartAction(user& client, vector<channel> &globalChannelList);
	/* ------------------------------------------------- */

public:

	PART();
	
	vector<reply>	execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);
	
	~PART();

};

#endif