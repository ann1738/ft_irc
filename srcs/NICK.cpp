#include "NICK.hpp"
#include <iostream>

NICK::NICK() {}

NICK::~NICK() {}

/**
 * @REMINDER: check if new nickname length is greater than 9
*/
void NICK::changeNickname(char* buffer, std::string nickname, User &user, int fd) {
	std::stringstream temp(buffer);
	std::string client_msg, new_nickname;

	temp >> client_msg;
	temp >> new_nickname;

	std::cout << temp.str() << std::endl;
	if (client_msg == "NICK" && !temp) {
		std::stringstream res;
		res << ":" << nickname << " NICK " << new_nickname << "\r\n";
		user.setNickname(new_nickname);
		send(fd, res.str().c_str(), strlen(res.str().c_str()), 0);
	}
}
