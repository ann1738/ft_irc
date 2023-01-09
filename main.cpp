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

	try {
		initialParse init;
		init.parse(argc, argv);

		server ircserv(init.getPort(), init.getPassword());
		getServerPtr(&ircserv);
		ircserv.run();
	} catch (std::exception &e) {
		cout << e.what() << endl;
		return 1;
	}
	return 0;
}
