#include "command.hpp"

command::command(): cmd_type(), parameters(), client() {}

string	command::getCmdType() const{
	return (this->cmd_type);
}

string	command::getParameters() const{
	return (this->parameters);
}

user&	command::getClient() const{
	return (*this->client);
}

user*	command::getClientPtr() const{
	return (this->client);
}

void	command::setCmdType(string t){
	this->cmd_type = t;
}

void	command::setParameters(string p){
	this->parameters = p;
}

void	command::setClient(user& c){
	this->client = &c;
}


command::~command() {}
