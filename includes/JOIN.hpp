#ifndef JOIN_HPP
# define JOIN_HPP

// using namespace std;

#include <iostream>
#include <vector>

#include "user.hpp"
#include "channel.hpp"
#include "command.hpp"

class JOIN {

private:
	vector<string>	channel_names;
	vector<string>	keys;
	user			client;

	size_t	getEnd(const string& s, size_t start);
	void	saveChannelNames(const string& parameters);
	void	saveKeys(const string& parameters);
	void	parseCmdParameters(const string& parameters);

	void	testingPars() const; // will be deleted in las editing;

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
