#ifndef NICK_HPP
#define NICK_HPP

#include "user.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class NICK {

private:
	vector<string> parseMessage(char* buffer) const;
	int getUserIndex(const vector<user>& users, int fd) const;
	bool isNicknameValid(const string& nickname) const;
	bool isNicknameTaken(const vector<user>& users, const string& nickname) const;
	void sendNumericResponse(int fd, int error_type, vector<string>& nickname);
	string buildResponse(const string& old_nickname, const string& new_nickname);
	void changeNickname(user& user, const string& new_nickname);

public:
	NICK();
	~NICK();

	void doNickCommand(vector<user>& users, int fd, char* buffer);

};

#endif
