#ifndef NICK_HPP
#define NICK_HPP

#include "reply.hpp"
#include "command.hpp"

class NICK {

private:
	bool                                        isNicknameValid(const string& nickname) const;
	pair<bool, vector<user>::const_iterator>    isNicknameTaken(const vector<user>& users, const string& nickname) const;
	string                                      buildResponse(const command &msg, const vector<user>& userList, const string& new_nickname);

	vector<reply>								buildReplies(const command &msg, vector<user> &globalUserList, string response);

public:
	NICK();
	~NICK();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
