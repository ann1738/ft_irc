#include "commandParse.hpp"

commandParse::commandParse() {}

size_t	commandParse::extractCmdType(const string &buff, size_t start, size_t i) {
	size_t	end = (buff.find(" ", start) < buff.find('\n', start))? buff.find(" ", start): buff.find('\n', start);

	this->cmd[i].setCmdType(buff.substr(start, (end - start)));
	return (end);
}

size_t	commandParse::extractParameters(const string &buff, size_t start, size_t i) {
	size_t	end = buff.find('\n', start);

	this->cmd[i].setParameters(buff.substr(start, (end - start)));
	return (end);
}

void	commandParse::saveNewCommand(const string &buff, size_t start, size_t i, user& u) {
	this->cmd.push_back(value_type());
	size_t	temp = extractCmdType(buff, start, i);
	if (temp != buff.find('\n', start))
		temp = extractParameters(buff, (temp + 1), i);
	this->cmd[i].setClient(u);
}

void	commandParse::parse(const string &buff, user& u) {
	size_t i = 0;
	for (size_t start = 0; start < buff.size(); start = (!start)? 0: (start + 1)) {
		size_t end = buff.find('\n', start);
		if (end == (size_t)(-1))
			break ;

		this->saveNewCommand(buff, start, i, u);

		start = end;
		i++;
	}
}

size_t		commandParse::getCommandAmount() const{
	return (this->cmd.size());
}

commandParse::value_type	commandParse::getParsedCmd(size_t i) const {
	return (this->cmd[i]);
}

void	commandParse::test() const {
	for (size_t i = 0; i < cmd.size(); i++) {
		cout << "**************************" << endl;
		cout << GREEN << "Command type = " << getParsedCmd(i).cmd_type << WHITE << endl;
		cout << BLUE << "Parameters = " << getParsedCmd(i).parameters << WHITE << endl;
		cout << YELLOW << "Client nickname = " << getParsedCmd(i).client->getNickname() << WHITE << endl;
		cout << "**************************" << endl;
	}
}

commandParse::~commandParse(){}
