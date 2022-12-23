#include "../includes/cmdPars.hpp"

cmdPars::cmdPars(const std::string buff){
	;
}

std::string	cmdPars::getCmdType() const{
	return (cmd.cmd_type);}

std::vector<std::string>	cmdPars::getParameters() const{
	return (cmd.parameters);}

std::string	cmdPars::getClientName() const{
	return (cmd.client_name);}

/*

buff will return 3-4 lines
first line is for the command
second line only god know what it is
third line could ether be (WHOIS client_name) || (client_name client_name ip client_name)
fourth line could be empty or have (ip client_name)


----- steps for command type -----
1- find first space 2- take everything till that space 3- put it in cmd_type variable.

------ steps for parameters ------
1- start from after that space and find next ( space || comma || colon ) 2- push-back in the parameters vector 3- repeat the cycle till you reach new line

------ steps for client name -----
1- starting from after the new line find the next new line 2- then from there find ( WHOIS || space ) 3- save everything after that space till another space or a new line in client_name

*/
