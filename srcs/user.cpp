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

void User::setNickname(std::string nickname) {
	this->m_nickname = nickname;
}

std::string User::getNickname() {
	return this->m_nickname;
}
