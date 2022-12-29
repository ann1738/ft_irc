#ifndef PART_HPP
#define PART_HPP

#include <iostream>
#include <vector>

#include "../user.hpp"
#include "../channel.hpp"
#include "../command.hpp"

#define PART_CORRECT(client_nickname, channel_name) (":" + client_nickname + " PART #" + channel_name)

#define P_ERR_NOSUCHCHANNEL(server_name, client_name, channel_name) (":" + server_name + " " + server_name + " 403 " + client_name + " #" + channel_name + " :No such channel")
#define P_ERR_NOTONCHANNEL(server_name, client_name, channel_name) (":" + server_name + " 442 " + client_name + " #" + channel_name + " :You're not on that channel")

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