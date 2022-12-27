#ifndef	COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include "user.hpp"

struct command {

private:
	std::string	cmd_type;
	std::string	parameters;
	user*		client;

	void	setCmdType(std::string s);
	void	setParameters(std::string p);
	void	setClient(user& c);

public:
	command();

	std::string	getCmdType() const;
	std::string	getParameters() const;
	user&		getClient() const;

	friend class commandParse;

	~command();

};

#endif