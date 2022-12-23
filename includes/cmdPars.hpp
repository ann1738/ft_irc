#ifndef CMDPARSE_HPP
# define CMDPARSE_HPP

#include <iostream>
#include <vector>

struct Command {

public:
	std::string					cmd_type;
	// std::vector<std::string>	cmd_type;
	std::vector<std::string>	parameters;
	std::string					client_name;
	// std::vector<std::string>	prefix;

};

class cmdPars {

public:
	typedef	Command				value_type;

private:
	value_type	cmd;
	cmdPars();

public:
	cmdPars(const std::string buff);

	std::string					getCmdType() const;
	std::vector<std::string>	getParameters() const;
	std::string					getClientName() const;

};

#endif