#include "NICK.hpp"

NICK::NICK() {}

NICK::~NICK() {}

NICK::NICK(user users) : m_user_list(users) {}

string NICK::buildResponse(string old_nickname, string new_nickname) {
	stringstream response;

	response << ":" << old_nickname << " NICK " << new_nickname << "\r\n";
	return response.str();
}

bool NICK::nicknameIsValid(string nickname) {
	string alpha("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
	       digits("0123456789"),
	       special("`|-_^{}[]\\");

	return nickname.length() < 10 &&
	       nickname.find_first_not_of(alpha + digits + special) == string::npos;
}

void NICK::sendNumericResponse(int fd, int error_type, string nickname) {
	stringstream response;

	response << nickname << " :"
	         << (error_type == 431 ? "No nickname given" :
	            error_type == 432 ? "Erroneus nickname" : "Nickname is already in use")
	         << "\r\n";
	send(fd, response.str().c_str(), strlen(response.str().c_str()), 0);
}

void NICK::changeNickname(char* buffer, string old_nickname, user &user, int fd) {
	stringstream temp(buffer);
	string client_message, new_nickname, extra, response;

	temp >> client_message;
	temp >> new_nickname;
	temp >> extra;

	if (!client_message.compare("NICK")) {

		// ERR_NONICKNAMEGIVEN (431)
		if (new_nickname.empty()) {
			this->sendNumericResponse(fd, 431, new_nickname);
			return;
		}

		// ERR_ERRONEUSNICKNAME (432)
		if (!this->nicknameIsValid(new_nickname) || !extra.empty()) {
			this->sendNumericResponse(fd, 432, new_nickname);
			return;
		}

		response = this->buildResponse(old_nickname, new_nickname);
		send(fd, response.c_str(), strlen(response.c_str()), 0);
		user.setNickname(new_nickname);
	}
}
