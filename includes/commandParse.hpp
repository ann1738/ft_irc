#ifndef COMMANDPARSE_HPP
# define COMMANDPARSE_HPP

#include "command.hpp"
#include <vector>

/* ------------ Colors ------------ */
#define BLACK	"\033[0;30m"
#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define YELLOW	"\033[0;33m"
#define BLUE	"\033[0;34m"
#define PURPLE	"\033[0;35m"
#define CYAN	"\x1b[0;36m"
#define WHITE	"\033[0;37m"

class commandParse {

public:
	typedef	command				value_type;

private:
	vector<value_type>	cmd;

	size_t		extractCmdType(const string &buff, size_t start, size_t i);
	size_t		extractParameters(const string &buff, size_t start, size_t i);
	void		saveNewCommand(const string &buff, size_t start, size_t i, user& u);

public:
	commandParse();

	void		parse(const string &buff, user& u);
	value_type	getParsedCmd(size_t i) const;
	size_t		getCommandAmount() const;

	void		test() const; // will be removed in final cleaning 

	~commandParse();

};

#endif