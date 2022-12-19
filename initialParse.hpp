#ifndef INITIALPARSE_HPP
# define INITIALPARSE_HPP

#include <iostream>
#include <string>
#include <sstream>

#define NBR_ARGS_ERR "Parse Error: Invalid number of arguments"
#define INVLD_PORT_ERR "Parse Error: Invalid port number"

class initialParse
{
private:
	int			port;
	std::string password;

	bool	checkValidPort(std::string portArg);
	void	savePort(std::string portArg);
	void	savePassword(std::string pass);

public:
	initialParse();
	~initialParse();

	int				getPort();
	std::string		getPassword();
	
	void	parse(int argc, char **argv);
};

#endif