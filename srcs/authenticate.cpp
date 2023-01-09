#include "authenticate.hpp"

authenticate::authenticate(const commandParse &cmd, const string& serverPassword){
	this->correct_order = (cmd.getParsedCmd(0).getCmdType() == "PASS")? true: false;
	if (correct_order == true && (this->getPassword(cmd.getParsedCmd(0).getParameters()) == serverPassword)) {
		this->correct_password = true;
		this->msg = "";
	}
	else {
		this->correct_password = false;
		this->msg = ERR_PASSWDMISMATCH(cmd.getParsedCmd(0).getClient().getServername(), cmd.getParsedCmd(0).getClient().getNickname());
	}
}

string	authenticate::getPassword(const string& parameters){
	int start = (parameters.find(' ') == string::npos)? 0 :(parameters.find_last_of(' ') + 1);
	int end = parameters.find('\r');
	return (parameters.substr(start, (end - start)));
}

bool	authenticate::isAuthenticated(){
	return ((correct_order == true) && (correct_password == true));
}

string	authenticate::getErrorMsg() const{
	return (this->msg);

}

authenticate::~authenticate(){}