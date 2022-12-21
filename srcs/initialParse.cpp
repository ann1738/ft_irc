#include "../includes/initialParse.hpp"

initialParse::initialParse()
{
	
}

initialParse::~initialParse()
{

}

int		initialParse::getPort(){
	return this->port;
}

std::string		initialParse::getPassword(){
	return this->password;
}

bool	initialParse::checkValidPort(std::string portArg){
	int					tempPort;
	std::stringstream	sstream;
	sstream << portArg;
	sstream >> tempPort;
	if (tempPort < 1023 || tempPort > 65535)
		return false;
	return true;
}

void	initialParse::savePort(std::string portArg){
	std::stringstream	sstream;
	sstream << portArg;
	sstream >> this->port;
}

void	initialParse::savePassword(std::string pass){
	this->password = pass;
}

void		initialParse::parse(int argc, char **argv){
	if (argc != 3)
	{
		std::cerr << NBR_ARGS_ERR << std::endl;
		exit(1);
	}

	if (checkValidPort(argv[1]) == false)
	{
		std::cerr << INVLD_PORT_ERR << std::endl;
		exit(1);
	}

	savePort(argv[1]);
	savePassword(argv[2]);
}
