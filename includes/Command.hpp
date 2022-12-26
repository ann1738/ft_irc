#ifndef	COMMAND_HPP
# define COMMAND_HPP

#include <iostream>

struct Command {

public:
	Command();

	std::string	cmd_type;
	std::string	parameters;
	std::string	remaining;

	~Command();

};

#endif