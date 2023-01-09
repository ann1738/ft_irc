#include "authenticate.hpp"

authenticate::authenticate(const commandParse &cmd, const string& serverPassword){
	this->correct_order = (cmd.getParsedCmd(0).getCmdType() =="PASS")? true: false;
	if (correct_order == true && (this->getPassword(cmd.getParsedCmd(0).getParameters()) == serverPassword))
		this->correct_Password = true;
	else
		this->correct_Password = false;
}

string	authenticate::getPassword(const string& parameters){
	int start = (parameters.find(' ') == (size_t)(-1))? 0 :(parameters.find_last_of(' ') + 1);
	int end = parameters.find('\r');
	return (parameters.substr(start, (end - start)));
}

bool	authenticate::isAuthenticated(){
	return ((correct_order == true) && (correct_Password == true))
}

authenticate::~authenticate(){}