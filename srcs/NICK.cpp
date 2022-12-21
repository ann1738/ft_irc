#include "NICK.hpp"
#include <iostream>

NICK::NICK() {}

NICK::~NICK() {}

int NICK::getIndex(std::vector<User> users, int fd) {
	for (int i = 0; i < static_cast<int>(users.size()); i++) {
		if (users[i].getFd() == fd) {
			return i;
		}
	}
	return -1;
}

/**
 * @REMINDER: check if new nickname length is greater than 9
*/
void NICK::changeNickname(char* buffer, std::vector<User> &users, int fd) {
	std::stringstream temp(buffer);
	std::string client_msg, new_nickname;

	temp >> client_msg;
	temp >> new_nickname;

	if (client_msg == "NICK") {
		std::stringstream res;
		int i = this->getIndex(users, fd);
		// if (i == -1) {
			
		// }

		res << ":" << users[i].getNickname() << " NICK " << new_nickname << "\r\n";
		users[i].setNickname(new_nickname);
		send(fd, res.str().c_str(), strlen(res.str().c_str()), 0);
	}
}
