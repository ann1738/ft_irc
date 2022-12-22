#ifndef USER_HPP
#define USER_HPP

#include <vector>
#include <cstring>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class user {

private:
	int               m_fd;
	string            m_hostname;
	string            m_nickname;
	string            m_mode;
	vector<string>    m_channels;

public:
	user();
	user(int fd);
	~user();

	int getFd();
	void initNickname(int count);
	void setNickname(string nickname);
	string getNickname();
};

#endif
