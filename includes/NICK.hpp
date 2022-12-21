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

		int getIndex(std::vector<User> users, int fd);
		void changeNickname(char* buffer, std::vector<User> &users, int fd);

};

#endif
