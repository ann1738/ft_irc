#ifndef AUTHENTICATE_HPP
# define AUTHENTICATE_HPP

#include "commandParse.hpp"

#define ERR_PASSWDMISMATCH(server_name, client_name) (":" + server_name + " 464 " + client_name + " :Password incorrect")

class authenticate {

private:

	bool	authenticated;
	string	error_msg;

	string	getLastParameter(const string &parameters);

public:

	authenticate(const commandParse &cmd, string serverPassword);

	bool	isAuthenticated() const;
	string	getErrorMsg() const;

	~authenticate();

};

#endif
