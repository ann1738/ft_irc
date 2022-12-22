#ifndef NICK_HPP
#define NICK_HPP

#include "user.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

class NICK {

	public:
		NICK();
		~NICK();

		std::string buildResponse(std::string nickname, std::string new_nickname);
		void changeNickname(char* buffer, std::string nickname, user &user, int fd);

};

#endif
