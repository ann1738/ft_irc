#ifndef USER_HPP
#define USER_HPP

#include <vector>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_NAME    "WeBareBears"

#define RPL_WELCOME(nickname) \
("001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "\n")

using namespace std;

class user {

private:
	int               m_fd;
	string            m_username;
	string            m_hostname;
	string            m_servername;
	string            m_realname;
	string            m_nickname;
	vector<string>    m_channels;
	bool              m_entered_server;

	vector<string> parseMessage(char* buffer) const;
	void initNickname();

public:
	user();
	~user();

	user(int fd);
	int getFd() const;

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

	void addChannel(const string& channel_name);
	void removeChannel(const string& channel_name);
	void enterServer();
};

#endif
