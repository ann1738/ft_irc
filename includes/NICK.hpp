#ifndef NICK_HPP
#define NICK_HPP

#include "user.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class NICK {

	public:
		NICK();
		~NICK();

		string buildResponse(string nickname, string new_nickname);
		void changeNickname(char* buffer, string nickname, user &user, int fd);

};

#endif
