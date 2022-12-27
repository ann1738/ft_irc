#ifndef	COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include "user.hpp"

using namespace std;

class command {

private:
	string	cmd_type;
	string	parameters;
	user*	client;

	void	setCmdType(string s);
	void	setParameters(string p);
	void	setClient(user& c);

public:
	command();

	string	getCmdType() const;
	string	getParameters() const;
	user&	getClient() const;

	friend class commandParse;

	~command();

};

#endif
