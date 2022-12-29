#include "commandParse.hpp"

commandParse::commandParse(): cmd() {}

size_t	commandParse::extractCmdType(const string &buff) {
	size_t	end = (buff.find(" ") < buff.find_last_of("\r\n"))? buff.find(" "): buff.find_last_of("\r\n");

	this->cmd.setCmdType(buff.substr(0, end));
	return (end);
}

size_t	commandParse::extractParameters(const string &buff, size_t start) {
	size_t	end = buff.find_last_of("\r\n");

	this->cmd.setParameters(buff.substr(start, (end - start)));
	return (end);
}

void	commandParse::parse(const string &buff, user& u) {
	size_t	i = extractCmdType(buff);
	if (i != buff.find_last_of("\r\n"))
		i = extractParameters(buff, (i + 1));
	this->cmd.setClient(u);
}

commandParse::value_type	commandParse::getParsedCmd() const {
	return (cmd);
}

void	commandParse::test() const {
	cout << GREEN << "Command type = " << getParsedCmd().cmd_type << WHITE << endl;
	cout << BLUE << "Parameters = " << getParsedCmd().parameters << WHITE << endl;
	cout << YELLOW << "Client nickname = " << getParsedCmd().client->getNickname() << WHITE << endl;
}

commandParse::~commandParse(){}
