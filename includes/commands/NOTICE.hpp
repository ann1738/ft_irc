#ifndef NOTICE_HPP
#define NOTICE_HPP

#include "reply.hpp"
#include "command.hpp"

string    getRecipient(string&);
bool      isNicknameJustSpaces(const string&);
bool      isNicknameInServer(const vector<user>&, const string&);
bool      isNicknameInChannel(const vector<const user*>&, const string&);
bool      isRecipientAChannel(const string&);
bool      canClientMessageChannel(const user&, const channel&);
size_t    getChannelIndex(const vector<channel>&, string);
size_t    getUserIndex(const vector<user>&, string);
pair<bool, const vector<channel>::const_iterator>    findChannel(const vector<channel>&, const string&);

class NOTICE {

private:
	void      buildUserResponse(stringstream& response, const user& client, const vector<user>& users,
	                            const string& nickname, const string& message);
	void      buildChannelResponse(stringstream& response, const user& client, const vector<channel>& channels,
	                               const string& channel_name, const string& message);
	string    buildResponse(const command& msg, const vector<user>& users, const vector<channel>& channels,
	                        const string& recipient, const string& message);
	void      setDestination(const user& client, const vector<user>& users, const vector<channel>& channels,
	                         vector<reply>& ret, const string& recipient);

public:
	NOTICE();
	~NOTICE();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
