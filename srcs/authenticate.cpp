#include "authenticate.hpp"

authenticate::authenticate(const commandParse &cmd, string serverPassword): error_msg(""){
	if (cmd.getCommandAmount()) {
		if (cmd.getParsedCmd(0).getCmdType() == "PASS") {
			if (this->getLastParameter(cmd.getParsedCmd(0).getParameters()) == serverPassword)
				this->authenticated = true;
		}
	}
	else {
		this->error_msg = ERR_PASSWDMISMATCH(cmd.getParsedCmd(0).getClient().getServername(), cmd.getParsedCmd(0).getClient().getNickname());
		this->authenticated = false;
	}
}

string	authenticate::getLastParameter(const string &parameters){
	int start = (parameters.find(' ') == (size_t)(-1))? 0 :(parameters.find_last_of(' ') + 1);
	int end = parameters.find('\r');
	return (parameters.substr(start, (end - start)));
}

bool	authenticate::isAuthenticated() const {
	return (this->authenticated);
}

string	authenticate::getErrorMsg() const {
	return (this->error_msg);
}

authenticate::~authenticate() {}
