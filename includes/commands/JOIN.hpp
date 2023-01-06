#ifndef JOIN_HPP
# define JOIN_HPP

#include "reply.hpp"
#include "command.hpp"

class JOIN {

private:
	vector<string>	channel_names;
	vector<string>	keys;

	/* -------------------- parsing -------------------- */
	size_t	getEnd(const string& s, size_t start);
	void	saveChannelNames(const string& parameters);
	void	saveKeys(const string& parameters);
	void	parseCmdParameters(const string& parameters);

	void	testingPars() const; // will be deleted in last editing;
	/* ------------------------------------------------- */

	/* ------------------- executing ------------------- */
	bool	isInviteError(user& client, vector<channel> &globalChannelList, size_t i) const;
	bool	isKeyError(vector<channel> &globalChannelList, size_t i, size_t position) const;
	bool	isChannelLimitError(vector<channel> &globalChannelList, size_t i) const;
	// bool	userLimitError();
	pair<size_t, string>	goThroughErrors(user& client, size_t position, vector<channel> &globalChannelList);
	vector<reply>			doJoinAction(user& client, vector<channel> &globalChannelList);
	/* ------------------------------------------------- */

public:

	JOIN();

	vector<reply>	execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

	~JOIN();

};

#endif
