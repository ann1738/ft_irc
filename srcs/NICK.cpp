#include "NICK.hpp"

NICK::NICK()
{
}

NICK::~NICK()
{
}

/**
 * splits the buffer into individual strings, which will be stored in a vector
*/
vector<string> NICK::parseMessage(char* buffer) const {
	stringstream temp(buffer);
	vector<string> client_message;
	string placeholder;

	while (temp >> placeholder) {
		client_message.push_back(placeholder);
	}
	return client_message;
}

/**
 * searches for a specific user by comparing its file descriptor with the
 * file descriptor provided by the server class
*/
int NICK::getUserIndex(const vector<user>& users, int fd) const {
	int i = 0;
	for (vector<user>::const_iterator iter = users.begin(); iter != users.end(); iter++, i++) {
		if (iter->getFd() == fd) {
			return i;
		}
	}
	return 0;
}

/**
 * checks if the desired nickname only consists of valid characters and if its length
 * does not exceed 9 characters
*/
bool NICK::isNicknameValid(const string& nickname) const {
	string alpha("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
	       digits("0123456789"),
	       special("`|-_^{}[]\\");

	return nickname.length() < 10 &&
	       nickname.find_first_not_of(alpha + digits + special) == string::npos;
}

/**
 * checks every user's nickname in the server and returns true if the desired nickname is
 * already being used or false if the desired nickname is available for use
*/
bool NICK::isNicknameTaken(const vector<user>& users, const string& nickname) const {
	for (vector<user>::const_iterator iter = users.begin(); iter != users.end(); iter++) {
		if (!iter->getNickname().compare(nickname)) {
			return true;
		}
	}
	return false;
}

/**
 * sends a response to the client in the following formats
 *
 * If the server does not receive the <nickname> parameter with the NICK command.
 * ERR_NONICKNAMEGIVEN (431)
 * " :No nickname given\r\n"
 *
 * If the server does not accept the new nickname supplied by the client as valid
 * (for instance, due to containing invalid characters).
 * ERR_ERRONEUSNICKNAME (432)
 * "<nick> :Erroneus nickname\r\n"
 *
 * If the server receives a NICK command from a client where the desired nickname
 * is already in use on the network.
 * ERR_NICKNAMEINUSE (433)
 * "<nick> :Nickname is already in use\r\n"
*/
void NICK::sendNumericResponse(int fd, int error_type, vector<string>& nickname) {
	stringstream response;

	nickname.erase(nickname.begin());
	while (!nickname.empty()) {
		response << nickname.front() << " ";
		nickname.erase(nickname.begin());
	}

	response << (error_type == 431 ? " :No nickname given" :
	            error_type == 432 ? ":Erroneus nickname" : ":Nickname is already in use")
	         << "\r\n";
	send(fd, response.str().c_str(), strlen(response.str().c_str()), 0);
}

/**
 * builds a string in the following format
 * ":<old nickname> NICK <new nickname>\r\n"
*/
string NICK::buildResponse(const string& old_nickname, const string& new_nickname) {
	stringstream response;
	response << ":" << old_nickname << " NICK " << new_nickname << "\r\n";
	return response.str();
}

/**
 * informs the client that it has been issued a new nickname
 * and then updates the user's nickname in the user class
*/
void NICK::changeNickname(user& user, const string& new_nickname) {
	string response = this->buildResponse(user.getNickname(), new_nickname);
	send(user.getFd(), response.c_str(), strlen(response.c_str()), 0);
	user.setNickname(new_nickname);
}

void NICK::doNickCommand(vector<user>& users, int fd, char* buffer) {
	user User = users[this->getUserIndex(users, fd)];
	User.saveUserInfo(buffer);

	vector<string> client_message = this->parseMessage(buffer);

	// checks if the command received from the client is /nick
	if (client_message[0].compare("NICK") ||
	   (client_message.size() > 2 && !client_message[2].compare("USER"))) {    // temporary fix for mac issue
		return;
	}

	/**
	 * prevents a nickname that only contains spaces
	 * example: /nick "   "
	*/
	else if (client_message.size() == 1) {
		this->sendNumericResponse(fd, 431, client_message);
	}

	/**
	 * checks if the desired nickname only consists of valid characters or
	 * if the parameters received from the client is equal to two
	 * example: /nick "hello world !"
	*/
	else if (!this->isNicknameValid(client_message[1]) || client_message.size() > 2) {
		this->sendNumericResponse(fd, 432, client_message);
	}

	// ensures that every user connected to the server will have a unique nickname
	else if (this->isNicknameTaken(users, client_message[1])) {
		this->sendNumericResponse(fd, 433, client_message);
	}

	// change a user's nickname if no error(s) have been encountered
	else {
		this->changeNickname(users[this->getUserIndex(users, fd)], client_message[1]);
	}
}
