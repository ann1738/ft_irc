#ifndef	cOMMAND_HPP
# define cOMMAND_HPP

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