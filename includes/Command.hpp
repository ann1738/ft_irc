#ifndef	COMMAND_HPP
# define COMMAND_HPP

#include <iostream>

struct command {

public:
	command();

	std::string	cmd_type;
	std::string	parameters;
	std::string	remaining;

	~command();

};

#endif