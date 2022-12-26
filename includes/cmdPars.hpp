#ifndef CMDPARSE_HPP
# define CMDPARSE_HPP

#include "Command.hpp"

class cmdPars {

public:
	typedef	Command				value_type;

private:
	value_type	cmd;

	size_t		extractCmdType(const std::string buff);
	size_t		extractParameters(const std::string buff, size_t start_i);
	size_t		extractRemaining(const std::string buff, size_t start);

public:
	cmdPars();


	void		parse(const std::string buff);
	value_type	getParsedCmd() const;

	~cmdPars();

};

#endif