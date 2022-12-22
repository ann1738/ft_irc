#include "NICK.hpp"

NICK::NICK() {}

NICK::~NICK() {}

std::string NICK::buildResponse(std::string old_nickname, std::string new_nickname) {
	std::stringstream response;
	response << ":" << old_nickname << " NICK " << new_nickname << "\r\n";
	return response.str();
}

/**
 * @REMINDER: check if new nickname length is greater than 9
*/
void NICK::changeNickname(char* buffer, std::string old_nickname, User &user, int fd) {
	std::stringstream temp(buffer);
	std::string client_message, new_nickname, response;

	temp >> client_message;
	temp >> new_nickname;

	if (!client_message.compare("NICK")) {
		response = this->buildResponse(old_nickname, new_nickname);
		user.setNickname(new_nickname);
		send(fd, response.c_str(), strlen(response.c_str()), 0);
	}
}
