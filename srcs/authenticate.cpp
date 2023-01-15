#include "authenticate.hpp"

authenticate::authenticate(const commandParse &cmd, const string& serverPassword){
	if (getCommandIndex(cmd, "NICK") != string::npos && getCommandIndex(cmd, "USER") != string::npos) {
		checkPass(cmd, serverPassword);
	}
	else if ((getCommandIndex(cmd, "NICK") == string::npos || getCommandIndex(cmd, "USER") == string::npos) && getCommandIndex(cmd, "PASS") != string::npos)
		authenticated = NOT_AUTHENTICATED;
	else
		authenticated = UNDETERMINED;
}

size_t	authenticate::getCommandIndex(const commandParse &cmd, string cmd_type){
	for (size_t i = 0; i < cmd.getCommandAmount(); i++){
		if (cmd.getParsedCmd(i).getCmdType() == cmd_type)
			return (i);
	}
	return (string::npos);
}

bool	authenticate::isOrderCorrect(const commandParse &cmd){
	return ((getCommandIndex(cmd, "USER") > getCommandIndex(cmd, "NICK")) && \
	(getCommandIndex(cmd, "NICK") > getCommandIndex(cmd, "PASS")));
}

void	authenticate::checkPass(const commandParse &cmd, const string& serverPassword){
	size_t pass_index = getCommandIndex(cmd, "PASS");
	if ((pass_index == string::npos) || !isOrderCorrect(cmd) || (serverPassword != getPassword(cmd.getParsedCmd(pass_index).getParameters()))){
		authenticated = NOT_AUTHENTICATED;
		msg = ERR_PASSWDMISMATCH(cmd.getParsedCmd(0).getClient().getServername(), cmd.getParsedCmd(0).getClient().getNickname());
	}
	else
		authenticated = AUTHENTICATED;
}

string	authenticate::getPassword(const string& parameters){
	int start = (parameters.find(' ') == string::npos)? 0 :(parameters.find_last_of(' ') + 1);
	int end = parameters.find('\r');
	return (parameters.substr(start, (end - start)));
}

int	authenticate::isAuthenticated(){
	return (authenticated);
}

string	authenticate::getErrorMsg() const{
	return (this->msg);
}

authenticate::~authenticate(){}