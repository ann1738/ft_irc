#include "initialParse.hpp"
#include "server.hpp"
#include <signal.h>

static server *getServerPtr(server *saveMe) {
	static server *savedPtr;
	return saveMe ? (savedPtr = saveMe) : savedPtr;
}

static void signalHandler(int sig) {
	(void)sig;
	getServerPtr(NULL)->terminateServer();
}

int main(int argc, char **argv)
{
	signal(SIGINT, signalHandler);
	signal(SIGQUIT, signalHandler);

	initialParse init;
	try {
		init.parse(argc, argv);
		cout << "Parsing done" << endl;
	} catch (std::exception &e) {
		cout << e.what() << endl;
		return 1;
	}

	try {
		server ircserv(init.getPort());
		getServerPtr(&ircserv);
		ircserv.run();
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}
	return 0;
}
