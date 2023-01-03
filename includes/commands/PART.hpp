#ifndef PART_HPP
#define PART_HPP

#include "reply.hpp"
#include "command.hpp"

class PART {

private:
	vector<string>	channel_names;

	/* -------------------- parsing -------------------- */
	size_t	getEnd(const string& s, size_t start);
	size_t	getStart(const string& s, size_t start);
	void	parseCmdParameters(const string& parameters);
	/* ------------------------------------------------- */

	/* ------------------- executing ------------------- */
	pair<size_t, string>	goThroughErrors(user& client, size_t position, vector<channel> &globalChannelList);
	string					doPartAction(user& client, vector<channel> &globalChannelList);
	/* ------------------------------------------------- */

public:

	PART();
	
	string	execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);
	
	~PART();

};

#endif