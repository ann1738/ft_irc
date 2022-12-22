#include "user.hpp"

User::User() {}

User::User(int fd) : m_fd(fd),
                     m_hostname(""),
                     m_nickname(""),
                     m_mode("") {}

User::~User() {}

int User::getFd() {
	return this->m_fd;
}

void User::initNickname(int count) {
	std::stringstream nickname;
	nickname << "guest_" << count;
	this->setNickname(nickname.str());
}

void User::setNickname() {
	char buffer[1024] = {0};

	read(m_fd, buffer, 1024);
	std::stringstream temp(buffer);
	std::string nickname;

	while (nickname != "NICK")
		temp >> nickname;
	temp >> nickname;
	this->m_nickname = nickname;
}

void User::setNickname(std::string nickname) {
	this->m_nickname = nickname;
}

std::string User::getNickname() {
	return this->m_nickname;
}
