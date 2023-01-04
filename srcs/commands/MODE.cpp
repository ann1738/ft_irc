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

void	MODE::dealWithAppropriateMode(){
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
				m_channel->setSecret(isPlus);
				break ;
			case 'p':
				m_channel->setPrivate(isPlus);
				break ;
			case 'i':
				m_channel->setInviteOnly(isPlus);
				break ;
			case 'n':
				m_channel->setNoExternalMsg(isPlus);
				break ;
			case 't':
				m_channel->setTopicSafe(isPlus);
				break ;
			case 'm':
				m_channel->setModerated(isPlus);
				break ;
			// case 'o':
			// 	handleModeO();
			// 	break ;
			// case 'v':
			// 	handleModeV();
			// 	break ;
			// case 'k':
			// 	handleModeK();
			// 	break ;
			case 'l':
				m_reply += handleModeL(isPlus);
				break ;
		} 
		pos++;
	}
}
//each mode may generate a reply


//atoi bec 22f would be taken as 22x
string	MODE::handleModeL(bool isPlus){
	if (isPlus == false)
	{
		m_channel->setUserCountLimit(0);
		m_channel->setUserCountLimited(false);
		return RPL_CHANNELMODEIS(m_user.getServername(), m_user.getNickname(), m_channel->getName(), string("-l"), string());
	}

	if (modeArgs.empty())
		return ERR_NEEDMOREPARAMS_MODE(m_user.getServername(), string("l"));
	cout << "after need more paramaters error" << endl;

	int limit;
	stringstream ss;
	ostringstream oss;

	ss << modeArgs[0];
	ss >> limit;
	
	if (limit <= 0)
		return "";

	if (limit > MAX_USER_COUNT_LIMIT)
		limit = MAX_USER_COUNT_LIMIT;

	oss << limit;

	m_channel->setUserCountLimited(true);
	m_channel->setUserCountLimit(limit);

	return RPL_CHANNELMODEIS(m_user.getServername(), m_user.getNickname(), m_channel->getName(), string("+l"), oss.str());
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

	/*	check that the user has privelage	*/
	if (isUserOperator(message.getClient()) == false)
		return ERR_CHANOPRIVSNEEDED_MODE(message.getClient().getServername(), message.getClient().getNickname(), parsedChannelName);
	/*Copy of user*/
	m_user = message.getClient();

	dealWithAppropriateMode();

	std::cout << endl << "\e[32m" << "END OF MODE::execute()" << "\e[0m" << endl; 
	return m_reply;
	
	// changeModes();
}
