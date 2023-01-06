#include "initialParse.hpp"
#include "server.hpp"
#include <signal.h>

void signalHandler(int sig) {
	(void)sig;
	throw std::runtime_error("\nWeBareBears IRC Server has been terminated.");
}

int main(int argc, char **argv)
{
	initialParse init;
	init.parse(argc, argv);

	std::cout << "Parsing done" << std::endl;
	signal(SIGINT, signalHandler);
	signal(SIGQUIT, signalHandler);
	try {
		server ircserv(init.getPort());
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}
}
