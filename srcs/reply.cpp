#include "reply.hpp"

reply::reply(): userFds(), msg("") {}
	
void	reply::setUserFds(const channel& c){
	for (size_t i = 0; i < c.getUsers().size(); i++) {
		this->userFds.push_back(c.getUsers()[i].getFd());
	}
}

void	reply::setUserFds(const channel& c, int fd){
	for (size_t i = 0; i < c.getUsers().size(); i++) {

		if (c.getUsers()[i].getFd() != fd)
			this->userFds.push_back(c.getUsers()[i].getFd());
	}
}

void	reply::setUserFds(const user& u){
	this->userFds.push_back(u.getFd());
}

void	reply::setMsg(const string& m){
	this->msg = m;
}

vector<int>	reply::getUserFds() const{
	return (this->userFds);
}

string		reply::getMsg() const{
	return (this->msg);
}

reply::~reply(){}