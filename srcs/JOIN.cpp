#include "JOIN.hpp"

JOIN::JOIN(){}

size_t	JOIN::getEnd(const string& s, size_t start = 0) {
	size_t end = s.find(',', start);

	if (end == (size_t)(-1) || ((s.find(' ', start) != (size_t)(-1)) && end > s.find(' ', start)))
		end = s.find(' ', start);
	
	if (end == (size_t)(-1) || ((s.find("\r\n", start) != (size_t)(-1)) && end > s.find("\r\n", start)))
		end = s.find("\r\n", start);
	return (end);
}

void	JOIN::saveChannelNames(const string& parameters) {
	for (size_t start = parameters.find('#'); start != (size_t)(-1); start = parameters.find('#', start)){
		size_t end = this->getEnd(parameters, start);
		start++;
		this->channel_names.push_back(parameters.substr(start, (end - start)));
	}
}

void	JOIN::saveKeys(const string& parameters){
	for (size_t start = parameters.find(' '); start != (size_t)(-1); start = parameters.find(',', start)){
		start++;
		size_t end = this->getEnd(parameters, start);
		this->keys.push_back(parameters.substr(start, (end - start)));
	}
}

void	JOIN::parseCmdParameters(const string& parameters){
	this->saveChannelNames(parameters);
	this->saveKeys(parameters);

	this->testingPars();
}

void	JOIN::testingPars() const {
	for (vector<string>::const_iterator it = this->channel_names.cbegin(); it != this->channel_names.cend(); it++)
		cout << "Channel: " << *it << endl;

	for (vector<string>::const_iterator it = this->keys.cbegin(); it != this->keys.cend(); it++)
		cout << "Key: " << *it << endl;
}

string	JOIN::execute(const command &msg, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	(void)globalChannelList;
	this->parseCmdParameters(msg.getParameters());
	
	return ("hi");
}

JOIN::~JOIN(){}


