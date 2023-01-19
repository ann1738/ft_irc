#ifndef MESSAGEUTILS_HPP
#define MESSAGEUTILS_HPP

#include "user.hpp"
#include "channel.hpp"

class messageUtils {

public:
	messageUtils();
	~messageUtils();

	string    getRecipient(string& buffer);

	bool      isNicknameJustSpaces(const string& nickname);
	bool      isNicknameInServer(const vector<user>& users, const string& nickname);
	bool      isNicknameInChannel(const vector<const user*>& users, const string& nickname);
	bool      isRecipientAChannel(const string& recipient);
	bool      canClientMessageChannel(const user& client, const channel& Channel);

	size_t    getChannelIndex(const vector<channel>& channels, string channel_name);
	size_t    getUserIndex(const vector<user>& users, string nickname);

	pair<bool, const vector<channel>::const_iterator>    findChannel(const vector<channel>& channels, const string& channel_name);

};

#endif
