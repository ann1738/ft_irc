#ifndef NICK_HPP
#define NICK_HPP

#include "user.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class NICK {

private:
	vector<string> parseMessage(char* buffer);
	int getUserIndex(vector<user> users, int fd);
	bool isNicknameValid(string nickname);
	bool isNicknameTaken(vector<user> users, string nickname);
	void sendNumericResponse(int fd, int error_type, vector<string> nickname);
	string buildResponse(string nickname, string new_nickname);
	void changeNickname(user& user, string new_nickname);

public:
	NICK();
	~NICK();

	void doNickCommand(vector<user>& users, int fd, char* buffer);

};

#endif
