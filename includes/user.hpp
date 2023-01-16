#ifndef USER_HPP
#define USER_HPP

#include <vector>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_NAME    "WeBareBears"

#define WEBAREBEARS "\00308                .- \00305     _________            __           ___________       __________ \n \
\00308       ,      /      \00305 |         '.         /  \\         |           '.    |          |   \n \
\00308     ,/  /   /  ,--. \00305 |    __     \\       /    \\        |     __      \\   |     _____|   \n \
\00308  ,-'/  /   /  /_,-' \00305 |   |__)    |      /      \\       |    |  \\     |   |    |___      \n \
\00308 (  /  /   /  |`     \00305 |          /      /   /\\   \\      |    |__/     /   |        |     \n \
\00308  '/  /._.'    '._.' \00305 |    ___    \\    /   /__\\   \\     |            /    |     ___|     \n \
\00308  |_,'               \00305 |   |___)    |  /            \\    |     __     \\    |    |_____    \n \
                    \00305  |           /  /     ____     \\   |    |  \\     \\   |          |   \n \
                    \00305  |_________,'  /____/      \\____\\  |____|   \\_____\\  |__________|   \n \
   ________       _________           ____           __________             ______       \n \
  /        '.    /         \\         /    \\         /          '.        ,-'      `-.    \n \
 |     _     \\  |       ___/        /      \\        |     _      \\      /       _    \\   \n \
 |    |_)    |  |      |__         /        \\       |    | \\     |      |      ( `-._/   \n \
 |           /  |         \\       /    /\\    \\      |    |_/     /       \\_     `'-.,    \n \
 |     _     \\  |       __/      /    /__\\    \\     |           /       _  '-.       \\   \n \
 |    |_)    |  |      |___     /              \\    |     .     (_,.   /  '-._)      |   \n \
 |           /  |          \\   /     ______     \\   |     |\\        )  \\             /   \n \
  \\________.'    \\_________/  (_____/      \\_____)  \\____/  `._____,'   '-._______,-'   \n \
\00314        _____       ________   ________   _____         _____  ________   ________       \n \
\00314     ,-'     `-.   /        \\ /        '. \\    \\       /    / /        \\ /        '.     \n \
\00314    /      _    \\ |      ___/ |    _     \\ \\    \\     /    / |      ___/ |    _     \\    \n \
\00314    |     ( `-._/ |     |__   |   | \\    |  \\    \\   /    /  |     |__   |   | \\    |    \n \
\00314     \\_    `'-.,  |        \\  |   |_/    /   \\    \\ /    /   |        \\  |   |_/    /    \n \
\00314    _  '-.      \\ |      __/  |         /     \\    V    /    |      __/  |         /     \n \
\00314   /  '-._)     | |     |___  |    .    (_,.   \\       /     |     |___  |    .    (_,.  \n \
\00314   \\            / |         \\ |    |\\       )   \\     /      |         \\ |    |\\       ) \n \
\00314    '-.______,-'   \\________/ \\___/  `.____,'    \\___/        \\________/ \\___/  `.____,' \n "

#define BEARS "\00305        _______________n_n_       \n \
\00305  _ ,-'                    \\_   \n \
\00305 (_/                   ' _' o\\   \n \
\00305   |                    (___) )  \n \
\00305   \\    |_______|    |____,.-'  \n \
\00305   |    |\00303_\00305|\00303_____\00305|    |\00303n_n_       \n \
\00303  _ \00305\\__/         \\__/\00303      \\_    \n \
\00303 (_d888          8888  @  @ .\\   \n \
\00303   y888          y88p     --  )  \n \
\00303   '888b_________d88b_____,.-'   \n \
\00303    Y888\00300_\00303|\00300_______\003038888\00300_n_n_       \n \
\00300  __\00303\"YP\"         \"YP\"\00300      \\_    \n \
\00300 (_/                   '  ' o\\   \n \
\00300   |                       -- )  \n \
\00300   \\    |________|    |___,.-'   \n \
\00300    |   | |      |    | |        \n \
\00300    \\___/-'       \\__/-'        \n"


#define RPL_WELCOME(nickname) \
("001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "\n")

using namespace std;

class user {

private:
	int               m_fd;
	string            m_username;
	string            m_hostname;
	string            m_servername;
	string            m_realname;
	string            m_nickname;
	vector<string>    m_channels;
	bool              m_entered_server;

	vector<string> parseMessage(char* buffer) const;
	void initNickname();

public:
	user();
	~user();

	user(int fd);
	int getFd() const;

	void saveUserInfo(char* buffer);

	void setUsername(string username);
	void setHostname(string hostname);
	void setServername(string servername);
	void setRealname(string realname);
	void setNickname(string nickname);

	string getUsername() const;
	string getHostname() const;
	string getServername() const;
	string getRealname() const;
	string getNickname() const;

	void addChannel(const string& channel_name);
	void removeChannel(const string& channel_name);
	void enterServer();
};

#endif
