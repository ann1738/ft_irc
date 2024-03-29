#include "initialParse.hpp"

initialParse::initialParse()
{
	
}

initialParse::~initialParse()
{

}

int				initialParse::getPort(){
	return this->port;
}

std::string		initialParse::getPassword(){
	return this->password;
}

bool			initialParse::checkValidPort(std::string portArg){
	int					tempPort;
	std::stringstream	sstream;
	sstream << portArg;

	if (portArg.find_first_not_of("0123456789") != std::string::npos)
		return false;
	sstream >> tempPort;
	if (tempPort < 1023 || tempPort > 65535)
		return false;
	return true;
}

void			initialParse::setPort(std::string portArg){
	std::stringstream	sstream;
	sstream << portArg;
	sstream >> this->port;
}

void			initialParse::setPassword(std::string pass){
	this->password = pass;
}

void			initialParse::parse(int argc, char **argv) throw(std::runtime_error){
	if (argc != 3)
		throw std::runtime_error(NBR_ARGS_ERR);

	if (checkValidPort(argv[1]) == false)
		throw std::runtime_error(INVLD_PORT_ERR);

	std::string alpha("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
	       digits("0123456789");

	if (std::string(argv[2]).length() < 3 || std::string(argv[2]).find_first_not_of(alpha + digits) != std::string::npos)
		throw std::runtime_error(INVLD_PASS_ERR);

	setPort(argv[1]);
	setPassword(argv[2]);
}
