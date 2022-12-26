#ifndef USER_HPP
#define USER_HPP

#include <vector>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class user {

private:
	int               m_fd;
	string            m_username;
	string            m_hostname;
	string            m_servername;
	string            m_realname;
	string            m_nickname;
	// string            m_mode;
	vector<string>    m_channels;

public:
	user();
	~user();

	user(int fd);
	int getFd();

	void initNickname();
	vector<string> parseMessage(char* buffer);
	void saveUserInfo(char* buffer);

	void setUsername(string username);
	void setHostname(string hostname);
	void setServername(string servername);
	void setRealname(string realname);
	void setNickname(string nickname);

	string getUsername() const;
	string getHostname() const;
	string getServername() const;
	string getRealname() const;
	string getNickname() const;

	void addChannel(string channel_name);
};

#endif
