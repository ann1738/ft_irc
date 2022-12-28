#ifndef JOIN_HPP
# define JOIN_HPP

// using namespace std;

#include <iostream>
#include <vector>

#include "user.hpp"
#include "channel.hpp"
#include "command.hpp"

#define JOIN_CORRECT(client_nickname, channel_name) (":" + client_nickname + " " + channel_name)
#define ERR_INVITEONLYCHAN(server_name, error_number, channel_name) (":" + server_name + " " + error_number + " * " + channel_name + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(server_name, error_number, channel_name) (":" + server_name + " " + error_number + " * " + channel_name + " :Cannot join channel (+i)")
#define ERR_CHANNELISFULL(server_name, error_number, channel_name) (":" + server_name + " " + error_number + " * " + channel_name + " :Cannot join channel (+i)")

class JOIN {

private:
	vector<string>	channel_names;
	vector<string>	keys;

	/* -------------------- parsing -------------------- */
	size_t	getEnd(const string& s, size_t start);
	void	saveChannelNames(const string& parameters);
	void	saveKeys(const string& parameters);
	void	parseCmdParameters(const string& parameters);

	void	testingPars() const; // will be deleted in las editing;
	/* ------------------------------------------------- */

	/* ------------------- executing ------------------- */
	bool	inviteError(user& client, vector<channel> &globalChannelList, size_t i) const;
	bool	keyError(vector<channel> &globalChannelList, size_t i, size_t position) const;
	bool	channelLimitError(vector<channel> &globalChannelList, size_t i) const;
	// bool	channelMaskError();
	// bool	userLimitError();
	pair<size_t, string>	goThroughErrors(user& client, size_t position, vector<channel> &globalChannelList);
	string					doJoinAction(user& client, vector<channel> &globalChannelList);
	/* ------------------------------------------------- */

public:

	JOIN();

	string	execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

	~JOIN();

};

/* -------------------------------------- *\
|*                 Steps:                 *|
|* 1- parse the parameters                *|
|* 2- Check for errors                    *|
|* 2- Add the channel in the client class *|
|* 2- Add the client in the channel class *|
|* 3- send the reply                      *|
\* -------------------------------------- */

#endif
