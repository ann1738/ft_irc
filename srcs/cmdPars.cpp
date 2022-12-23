#include "../includes/cmdPars.hpp"

cmdPars::cmdPars(): cmd() {}

size_t	cmdPars::extractCmdType(const std::string buff) {
	size_t	i = buff.find(" ");
	this->cmd.cmd_type = buff.substr(0, (i - 1));
	return (i);
}

size_t	cmdPars::extractParameters(const std::string buff, size_t start) {
	size_t	end = buff.find("\n");
	this->cmd.parameters = buff.substr(start, (end - 1));
	return (end);
}

size_t	cmdPars::extractClientName(const std::string buff) {
	size_t	start = buff.find("WHOIS");
	if (start == -1)
		return (0);
	size_t	end = buff.find("\n");
	if (end == -1)
		end = buff.find("\n\r");
	if (end == -1)
		this->cmd.parameters = buff.substr(start + 6);
	else
		this->cmd.parameters = buff.substr((start + 6), (end - 1));
	return (end);
}

void	cmdPars::parse(const std::string buff) {
	size_t	i = extractCmdType(buff);
	i = extractParameters(buff, (i + 1));
	extractClientName(buff);
}

cmdPars::value_type	cmdPars::getParsedCmd() const {
	return (cmd);
}

cmdPars::~cmdPars(){}
