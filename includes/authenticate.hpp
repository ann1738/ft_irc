#ifndef AUTHENTICATE_HPP
# define AUTHENTICATE_HPP

#include "commandParse.hpp"
#include "reply.hpp"

#define AUTHENTICATED 1
#define NOT_AUTHENTICATED -1
#define UNDETERMINED 0

class authenticate {

private:
	int		authenticated;
	string	msg;

	string	getPassword(const string& parameters);

	void	checkPass(const commandParse &cmd, const string& serverPassword);

	size_t	getNickIndex(const commandParse &cmd);
	size_t	getUserIndex(const commandParse &cmd);
	size_t	getPassIndex(const commandParse &cmd);

public:
	authenticate(const commandParse &cmd, const string& serverPassword);

	int	isAuthenticated();

	string	getErrorMsg() const;
	
	~authenticate();

};

#endif
