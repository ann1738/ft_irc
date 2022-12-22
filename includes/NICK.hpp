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

		void changeNickname(char* buffer, std::string nickname, User &user, int fd);

};

#endif
