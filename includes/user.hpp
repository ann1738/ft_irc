#ifndef USER_HPP
#define USER_HPP

#include <vector>
#include <string>
#include <sstream>

#include <unistd.h>

class User {

private:
	int                         m_fd;
	std::string                 m_hostname;
	std::string                 m_nickname;
	std::string                 m_mode;
	std::vector<std::string>    m_channels;

public:
	User();
	User(int fd);
	~User();

	int getFd();
	void setNickname();
	void setNickname(std::string nickname);
	std::string getNickname();
};

#endif
