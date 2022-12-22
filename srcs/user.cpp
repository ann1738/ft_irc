#include "user.hpp"

user::user() {}

user::user(int fd) : m_fd(fd),
                     m_hostname(""),
                     m_nickname(""),
                     m_mode("") {}

user::~user() {}

int user::getFd() {
	return this->m_fd;
}

void user::initNickname(int count) {
	stringstream nickname;
	nickname << "guest_" << count;
	this->setNickname(nickname.str());
}

void user::setNickname(string nickname) {
	this->m_nickname = nickname;
}

string user::getNickname() {
	return this->m_nickname;
}
