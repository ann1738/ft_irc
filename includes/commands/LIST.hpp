#ifndef LIST_HPP
#define LIST_HPP

#include "reply.hpp"
#include "command.hpp"

class LIST {

private:
	string           userCountToString(vector<channel>::const_iterator& channel);
	vector<string>   parseChannelNames(string& message);

	bool             doesChannelExist(const vector<string>& channels, const string& channel_name);
	bool             clientIsInsideChannel(const user& client, const vector<user>& userList);

	string           addTopic(vector<channel>::const_iterator it);
	void             addToResponse(const command &msg, vector<channel>::const_iterator it, stringstream& response);
	void             addChannelsToList(const command& msg, const vector<channel>& channelList, stringstream& response);
	string           getList(const command& msg, const vector<channel>& channelList);
	vector<reply>    buildResponse(const command& msg, const vector<channel> &channelList);

public:
	LIST();
	~LIST();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
