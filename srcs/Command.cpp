#include "command.hpp"

command::command(): cmd_type(), parameters(), client() {}

std::string	command::getCmdType() const{
	return (this->cmd_type);
}

std::string	command::getParameters() const{
	return (this->parameters);
}

user&	command::getClient() const{
	return (*this->client);
}

void	command::setCmdType(std::string t){
	this->cmd_type = t;
}

void	command::setParameters(std::string p){
	this->parameters = p;
}

void	command::setClient(user& c){
	this->client = &c;
}


command::~command() {}
