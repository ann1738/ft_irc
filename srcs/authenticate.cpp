#include "authenticate.hpp"

authenticate::authenticate(const commandParse &cmd, const string& serverPassword){
	if (getNickIndex(cmd) == string::npos && getUserIndex(cmd) == string::npos)
		authenticated = UNDETERMINED;
	else if (getNickIndex(cmd) != string::npos && getUserIndex(cmd) != string::npos) {
		checkPass(cmd, serverPassword);
	}
}

size_t	authenticate::getNickIndex(const commandParse &cmd){
	for (size_t i = 0; i < cmd.getCommandAmount(); i++){
		if (cmd.getParsedCmd(i).getCmdType() == "NICK")
			return (i);
	}
	return (string::npos);
}

size_t	authenticate::getUserIndex(const commandParse &cmd){
	for (size_t i = 0; i < cmd.getCommandAmount(); i++){
		if (cmd.getParsedCmd(i).getCmdType() == "USER")
			return (i);
	}
	return (string::npos);
}

size_t	authenticate::getPassIndex(const commandParse &cmd){
	for (size_t i = 0; i < cmd.getCommandAmount(); i++){
		if (cmd.getParsedCmd(i).getCmdType() == "PASS")
			return (i);
	}
	return (string::npos);
}

void	authenticate::checkPass(const commandParse &cmd, const string& serverPassword){
	size_t pass_index = getPassIndex(cmd);
	if ((pass_index == string::npos) || (serverPassword != getPassword(cmd.getParsedCmd(pass_index).getParameters()))){
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