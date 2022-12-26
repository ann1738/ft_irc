#ifndef CMDPARSeE_HPP
# define CMDPARSeE_HPP

#include "command.hpp"

/* ------------ Colors ------------ */
#define BLACK	"\033[0;30m"
#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define YELLOW	"\033[0;33m"
#define BLUE	"\033[0;34m"
#define PURPLE	"\033[0;35m"
#define CYAN	"\x1b[0;36m"
#define WHITE	"\033[0;37m"

class cmdParse {

public:
	typedef	command				value_type;

private:
	value_type	cmd;

	size_t		extractCmdType(const std::string &buff);
	size_t		extractParameters(const std::string &buff, size_t start_i);
	size_t		extractRemaining(const std::string &buff, size_t start);

public:
	cmdParse();


	void		parse(const std::string buff);
	value_type	getParsedCmd() const;

	void		test() const; // will be removed in final cleaning 

	~cmdParse();

};

#endif