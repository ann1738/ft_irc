#include "../includes/cmdPars.hpp"

cmdPars::cmdPars(): cmd() {}

size_t	cmdPars::extractCmdType(const std::string buff) {
	size_t	i = (buff.find(" ") < buff.find('\n'))? buff.find(" "): buff.find('\n');

	this->cmd.cmd_type = buff.substr(0, i);
	return (i);
}

size_t	cmdPars::extractParameters(const std::string buff, size_t start) {
	size_t	end = buff.find('\n');

	this->cmd.parameters = buff.substr(start, (end - start));
	return (end);
}

size_t	cmdPars::extractRemaining(const std::string buff, size_t start) {
	size_t end = buff.find("\n\r");
	this->cmd.remaining = buff.substr(start, (end - start));

	return (end);
}

void	cmdPars::parse(const std::string buff) {
	size_t	i = extractCmdType(buff);
	if (i != buff.find('\n'))
		i = extractParameters(buff, (i + 1));
	if (i != buff.find("\n\r"))
		extractRemaining(buff, (i + 1));
}

cmdPars::value_type	cmdPars::getParsedCmd() const {
	return (cmd);
}

cmdPars::~cmdPars(){}
