#include "../includes/cmdParse.hpp"

cmdParse::cmdParse(): cmd() {}

size_t	cmdParse::extractCmdType(const std::string &buff) {
	size_t	end = (buff.find(" ") < buff.find('\n'))? buff.find(" "): buff.find('\n');

	this->cmd.cmd_type = buff.substr(0, end);
	return (end);
}

size_t	cmdParse::extractParameters(const std::string &buff, size_t start) {
	size_t	end = buff.find('\n');

	this->cmd.parameters = buff.substr(start, (end - start));
	return (end);
}

size_t	cmdParse::extractRemaining(const std::string &buff, size_t start) {
	size_t end = buff.find("\r\n");
	this->cmd.remaining = buff.substr(start, (end - start));

	return (end);
}

void	cmdParse::parse(const std::string buff) {
	size_t	i = extractCmdType(buff);
	if (i != buff.find('\n'))
		i = extractParameters(buff, (i + 1));
	if (i != buff.find("\r\n"))
		extractRemaining(buff, (i + 1));
}

cmdParse::value_type	cmdParse::getParsedCmd() const {
	return (cmd);
}

void	cmdParse::test() const {
	std::cout << GREEN << "Command type = " << getParsedCmd().cmd_type << WHITE << std::endl;
	std::cout << BLUE << "Parameters = " << getParsedCmd().parameters << WHITE << std::endl;
	std::cout << YELLOW << "Remaining = " << getParsedCmd().remaining << WHITE << std::endl;
}

cmdParse::~cmdParse(){}
