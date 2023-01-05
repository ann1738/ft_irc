#include "MODE.hpp"

MODE::MODE(): parsedChannelName(""), parsedModes(""), m_reply(""), modeArgsIndex(0)
{
}

MODE::~MODE()
{
}

//make sure that the channel starts with #
void	MODE::parseChannelName(string &parameters){
	parsedChannelName = parameters.substr(0, parameters.find(' '));
	parameters.erase(0, parameters.find(' ') + 1);                        //erase the channel name
}

// take care of duplicate modes like ++iiii
void	MODE::parseModes(string &parameters){
	string	modesParam = parameters.substr(0, parameters.find(' '));           //extract the mode specification
	
	bool	isPlus = true;
	string::size_type	pos = 0;

	while ((pos = modesParam.find_first_of(MODES, pos)) != string::npos)        //loop until not more 
	{
		if (pos != 0 && (modesParam[pos - 1] == '-' || modesParam[pos - 1] == '+'))  //should I add or remove mode
			isPlus = (modesParam.at(pos - 1) == '+');
		parsedModes += (isPlus ? "+" : "-");
		parsedModes += modesParam[pos];
		pos++;
	}
	parameters.erase(0, parameters.find(' '));                        //erase the mode
	
}

void	MODE::storeModeArguments(string &parameters){
	stringstream	ss;
	string 			temp;
	ss << parameters;


	while (ss >> temp)
		modeArgs.push_back(temp);
	for (vector<string>::iterator it = modeArgs.begin(); it != modeArgs.end(); it++)
		std::cout << *it << std::endl;

}

bool	MODE::isChannel(const string& channelName, vector<channel> &globalChannelList){
	for (vector<channel>::iterator it = globalChannelList.begin(); it != globalChannelList.end(); ++it)
	{
		if ("#" + it->getName() == channelName)
			return true;
	}
	return false;
}

void	MODE::storeChannel(const string& channelName, vector<channel> &globalChannelList){
	for (vector<channel>::iterator it = globalChannelList.begin(); it != globalChannelList.end(); ++it)
	{
		if ("#" + it->getName() == channelName)
		{
			m_channel = &*it;
			break ;
		}
	}
}

void	MODE::changeModes(){

}

bool	MODE::isUserOperator(const user& User){
	return m_channel->isOperator(User);
}

void	MODE::dealWithAppropriateMode(vector<user> &globalUserList){
	char 				mode;
	bool				isPlus;
	string::size_type	pos = 0;
	while ((pos = parsedModes.find_first_of("+-", pos)) != string::npos)
	{
		isPlus = (parsedModes[pos] == '+') ? true : false;
		mode = parsedModes[pos + 1];

		switch (mode)
		{
			case 's':
				m_reply += handleModeS(isPlus);
				break ;
			case 'p':
				m_reply += handleModeP(isPlus);
				break ;
			case 'i':
				m_reply += handleModeI(isPlus);
				break ;
			case 'n':
				m_reply += handleModeN(isPlus);
				break ;
			case 't':
				m_reply += handleModeT(isPlus);
				break ;
			case 'm':
				m_reply += handleModeM(isPlus);
				break ;
			case 'o':
				m_reply += handleModeO(isPlus, globalUserList);
				break ;
			case 'v':
				m_reply += handleModeV(isPlus, globalUserList);
				break ;
			case 'k':
				m_reply += handleModeK(isPlus);
				break ;
			case 'l':
				m_reply += handleModeL(isPlus);
				break ;
		} 
		pos++;
	}
	modeArgsIndex = 0;
	std::cout << "\e[31mMODE::m_reply = " << m_reply << "\e[0m" << std::endl;
}
//each mode may generate a reply

string	MODE::handleModeS(bool isPlus){
	if (isPlus == m_channel->getSecret())
		return "";

	m_channel->setSecret(isPlus);

	string mode = isPlus ? "+" : "-";
	mode += "s";
	return RPL_CHANNELMODEIS(m_user->getNickname(), m_channel->getName(), mode, string());
}

string	MODE::handleModeP(bool isPlus){
	if (isPlus == m_channel->getPrivate())
		return "";

	m_channel->setPrivate(isPlus);

	string mode = isPlus ? "+" : "-";
	mode += "p";
	return RPL_CHANNELMODEIS( m_user->getNickname(), m_channel->getName(), mode, string());
}

string	MODE::handleModeI(bool isPlus){
	if (isPlus == m_channel->getInviteOnly())
		return "";

	m_channel->setInviteOnly(isPlus);

	string mode = isPlus ? "+" : "-";
	mode += "i";
	return RPL_CHANNELMODEIS(m_user->getNickname(), m_channel->getName(), mode, string());
}

string	MODE::handleModeM(bool isPlus){
	if (isPlus == m_channel->getModerated())
		return "";

	m_channel->setModerated(isPlus);

	string mode = isPlus ? "+" : "-";
	mode += "m";
	return RPL_CHANNELMODEIS(m_user->getNickname(), m_channel->getName(), mode, string());
}

string	MODE::handleModeN(bool isPlus){
	if (isPlus == m_channel->getNoExternalMsg())
		return "";

	m_channel->setNoExternalMsg(isPlus);

	string mode = isPlus ? "+" : "-";
	mode += "n";
	return RPL_CHANNELMODEIS(m_user->getNickname(), m_channel->getName(), mode, string());
}

string	MODE::handleModeT(bool isPlus){
	if (isPlus == m_channel->getTopicSafe())
		return "";

	m_channel->setTopicSafe(isPlus);

	string mode = isPlus ? "+" : "-";
	mode += "t";
	return RPL_CHANNELMODEIS(m_user->getNickname(), m_channel->getName(), mode, string());
}

string	MODE::handleModeL(bool isPlus){
	if (isPlus == false && m_channel->getUserCountLimited() == false) return "";
	if (isPlus == true && m_channel->getUserCountLimited() == true) return "";
	if (isPlus == false)
	{
		m_channel->setUserCountLimit(0);
		m_channel->setUserCountLimited(false);
		return RPL_CHANNELMODEIS( m_user->getNickname(), m_channel->getName(), string("-l"), string());
	}

	if (modeArgs.size() <= modeArgsIndex) //check condition again
		return ERR_NEEDMOREPARAMS_MODE(m_user->getServername(), string("l"));

	int limit;
	stringstream ss;
	ostringstream oss;

	ss << modeArgs[modeArgsIndex++];
	ss >> limit;
	
	if (limit <= 0)
		return "";

	if (limit > MAX_USER_COUNT_LIMIT)
		limit = MAX_USER_COUNT_LIMIT;

	oss << limit;

	m_channel->setUserCountLimited(true);
	m_channel->setUserCountLimit(limit);

	return RPL_CHANNELMODEIS( m_user->getNickname(), m_channel->getName(), string("+l"), oss.str());
}

/*So I am going to make my own system for the channel mode arguments where the parameters positions is dependent on the order of the mode presented*/

/* helper function */

vector<user>::iterator	MODE::findUser(vector<user> &userList, const string &nickname) {
	size_t	index = 0;
	for (vector<user>::iterator it = userList.begin(); it != userList.end(); it++, index++)
	{
		if (it->getNickname() == nickname)
			return it;
	}
	return userList.end();
}


string	MODE::handleModeO(bool isPlus, vector<user> &globalUserList){
	if (modeArgs.size() <= modeArgsIndex) //check condition again
		return ERR_NEEDMOREPARAMS_MODE(m_user->getServername(), string("o"));

	string nickname = modeArgs[modeArgsIndex++];

	vector<user>::iterator userIter = findUser(globalUserList, nickname);
	if (userIter == globalUserList.end())
		return ERR_NOSUCHNICK(m_user->getServername(), m_user->getNickname(), nickname);

	for (vector<user>::const_iterator it = m_channel->getUsers().begin(); it != m_channel->getUsers().end(); it++)
		cout << it->getNickname() << endl;
	if (m_channel->isUser(*userIter) == false)
		return "";
	
	if (m_user->getNickname() == userIter->getNickname()) //in case operator is trying to change the mode on herself
		return "";	

	if (isPlus == true && m_channel->isOperator(*userIter) == true)
		return "";

	if (isPlus == false && m_channel->isOperator(*userIter) == false)
		return "";

	/*give/take privilege*/		
	if (isPlus == true)
		m_channel->addOperator(*userIter);
	else
		m_channel->removeOperator(*userIter);

	/*construct reply*/		
	string mode = isPlus ? "+" : "-";
	mode += "o";
	return RPL_CHANNELMODEIS(m_user->getNickname(), m_channel->getName(), mode, modeArgs[modeArgsIndex - 1]);
}


string	MODE::handleModeV(bool isPlus, vector<user> &globalUserList){
	if (modeArgs.size() <= modeArgsIndex) //check condition again
		return ERR_NEEDMOREPARAMS_MODE(m_user->getServername(), string("v"));

	string nickname = modeArgs[modeArgsIndex++];

	vector<user>::iterator userIter = findUser(globalUserList, nickname);
	if (userIter == globalUserList.end())
		return ERR_NOSUCHNICK(m_user->getServername(), m_user->getNickname(), nickname);

	if (m_channel->isUser(*userIter) == false)
		return "";
	
	if (m_user->getNickname() == userIter->getNickname()) //in case operator is trying to change the mode on herself
		return "";	

	if (isPlus == true && m_channel->isVoicedUser(*userIter) == true)
		return "";

	if (isPlus == false && m_channel->isVoicedUser(*userIter) == false)
		return "";

	/*give/take privilege*/		
	if (isPlus == true)
		m_channel->addVoicedUser(*userIter);
	else
		m_channel->removeVoicedUser(*userIter);

	/*construct reply*/		
	string mode = isPlus ? "+" : "-";
	mode += "v";
	return RPL_CHANNELMODEIS(m_user->getNickname(), m_channel->getName(), mode, modeArgs[modeArgsIndex - 1]);
}

string	MODE::handleModeK(bool isPlus){
	if (isPlus == false && m_channel->getKeyEnabled() == false) return "";
	if (isPlus == true && m_channel->getKeyEnabled() == true) return "";
	if (isPlus == false)
	{
		m_channel->setKeyEnabled(false);
		m_channel->setKey("");
		return RPL_CHANNELMODEIS( m_user->getNickname(), m_channel->getName(), string("-k"), string());
	}

	cout << "before checking if parameters exist" << endl;
	if (modeArgs.size() <= modeArgsIndex) //check condition again
		return ERR_NEEDMOREPARAMS_MODE(m_user->getServername(), string("k"));

	string key = modeArgs[modeArgsIndex++];

	cout << "key: " << key << endl;
	if (key.size() < 3)
		return ERR_PASSTOOSMALL(m_user->getServername(), m_user->getNickname());
	
	
	m_channel->setKeyEnabled(true);
	m_channel->setKey(key);
	return RPL_CHANNELMODEIS( m_user->getNickname(), m_channel->getName(), string("+k"), key);
}


void	MODE::storeUser(const string& nickname, vector<user> &globalUserList){
	for (vector<user>::iterator it = globalUserList.begin(); it != globalUserList.end(); ++it)
	{
		cout << "storeUser()" << endl;
		if (it->getNickname() == nickname)
		{
			m_user = &*it;
			break ;
		}
	}
}


void	MODE::logModes(){
	string	modes;
	if (m_channel->getInviteOnly())
		modes += "i";
	if (m_channel->getTopicSafe())
		modes += "t";
	if (m_channel->getModerated())
		modes += "m"; 
	if (m_channel->getPrivate())
		modes += "p"; 
	if (m_channel->getSecret())
		modes += "s"; 
	if (m_channel->getKeyEnabled())
		modes += "k"; 
	if (m_channel->getUserCountLimited())
		modes += "l"; 
	if (m_channel->getNoExternalMsg())
		modes += "n";
	cout << endl << "\e[33mCurrent modes for #" << m_channel->getName() << " " << modes << "\e[0m" << endl;
}


string	MODE::execute(const command &message, vector<user> &globalUserList, vector<channel> &globalChannelList){
	(void)globalUserList;
	(void)m_user;
	(void)modeArgsIndex;
	std::cout << "\e[32m" << "START OF MODE::execute()" << "\e[0m" << endl << endl; 
	string parameters = message.getParameters();
	parseChannelName(parameters);
	parseModes(parameters);
	storeModeArguments(parameters);


	/*	check that the channel exists	*/
	if (isChannel(parsedChannelName, globalChannelList) == false)
		return ERR_NOSUCHCHANNEL_MODE(message.getClient().getServername(), message.getClient().getNickname(), parsedChannelName);
	storeChannel(parsedChannelName, globalChannelList);

	/*	check that the user has privilege	*/
	if (isUserOperator(message.getClient()) == false)
		return ERR_CHANOPRIVSNEEDED_MODE(message.getClient().getServername(), message.getClient().getNickname(), parsedChannelName);
	storeUser(message.getClient().getNickname(), globalUserList);

	dealWithAppropriateMode(globalUserList);

	logModes();

	std::cout << endl << "\e[32m" << "END OF MODE::execute()" << "\e[0m" << endl; 
	return m_reply;
	
	// changeModes();
}
