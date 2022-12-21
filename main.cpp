#include "includes/initialParse.hpp"
#include "includes/server.hpp"

int main(int argc, char **argv)
{
	initialParse init;
	init.parse(argc, argv);

	std::cout << "Parsing done" << std::endl;
	server ircserv(init.getPort());
}
