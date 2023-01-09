#ifndef LIST_HPP
#define LIST_HPP

#include "reply.hpp"
#include "command.hpp"

class LIST {

private:
	string           userCountToString(const vector<channel>::const_iterator& channel);
	vector<string>   parseChannelNames(const string& message);

	bool             doesChannelExist(const vector<string>& channels, const string& channel_name);
	bool             isClientInsideChannel(const vector<const user*>& users, const user& client);

	string           addMode(const vector<channel>::const_iterator& it);
	string           addTopic(const vector<channel>::const_iterator& it);
	void             addToResponse(const command& msg, const vector<channel>::const_iterator& it, stringstream& response);
	void             addChannelsToList(const command& msg, const vector<channel>& channelList, stringstream& response);
	string           getList(const command& msg, const vector<channel>& channelList);
	vector<reply>    buildResponse(const command& msg, const vector<channel> &channelList);

public:
	LIST();
	~LIST();

	vector<reply>    execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList);

};

#endif
