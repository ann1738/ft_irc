#include "reply.hpp"

reply::reply(): userFds(), msg("") {}
	
void	reply::setUserFds(channel& c){
	for (size_t i = 0; i < c.getUsers().size(); i++) {

		this->userFds.push_back(c.getUsers().at(i).getFd());
		cout << "fd => " << c.getUsers().at(i).getFd() << "is added to reply class" << endl;
	}
}

void	reply::setUserFds(user& u){
	this->userFds.push_back(u.getFd());
}

void	reply::setMsg(string m){
	this->msg = m;
}

vector<int>	reply::getUserFds() const{
	return (this->userFds);
}

string		reply::getMsg() const{
	return (this->msg);
}

reply::~reply(){}