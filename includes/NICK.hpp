#ifndef NICK_HPP
#define NICK_HPP

#include "user.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class NICK {

	private:
		user    m_user_list;

	public:
		NICK();
		~NICK();

		NICK(user users);
		bool nicknameIsValid(string nickname);
		void sendNumericResponse(int fd, int error_type, string nickname);
		string buildResponse(string nickname, string new_nickname);
		void changeNickname(char* buffer, string nickname, user &user, int fd);

};

#endif
