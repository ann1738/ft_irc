#ifndef AUTHENTICATE_HPP
# define AUTHENTICATE_HPP

#include "commandParse.hpp"
#include "reply.hpp"

class authenticate {

private:
	bool	correct_order;
	bool	correct_password;
	string	msg;

	string	getPassword(const string& parameters);

public:
	authenticate(const commandParse &cmd, const string& serverPassword);

	bool	isAuthenticated();

	string	getErrorMsg() const;
	
	~authenticate();

};

#endif

// #define ERR_ALREADYREGISTERED(servername, nickname) \
// (":" + servername + " 462 * " + nickname + " :You may not reregister\n")

// #define ERR_PASSWDMISMATCH(servername, nickname) \
// (":" + servername + " 464 * " + nickname + " :Password incorrect\n")

/* ----------------------------------------------------------------------------------------------------- *\
|*                              Step 1: check if user is already connected                               *|
|* - check the ip of the user and if it matches any other ip connected.                                  *|
|*   - if they are send an error <ERR_ALREADYREGISTERED> and close the newly opened fd.                  *|
|*   - else go to next step.                                                                             *|
|* ----------------------------------------------------------------------------------------------------- *|
|*                                       Step 2: look for password                                       *|
|* - the class will take all parsed commands.                                                            *|
|* - if the first command is CAP or JOIN it will ignore and not authenticate.                            *|
|* - else if the first command is NOT PASS send <ERR_PASSWDMISMATCH> an error and close newly opened fd. *|
|* - else if the first command is PASS go to next step.                                                  *|
|* ----------------------------------------------------------------------------------------------------- *|
|*                                   Step 3: check the password given                                    *|
|* - check the password given and compare it with the server password.                                   *|
|*   - if it's correct authenticate them, send welcome msg, and add the user to the server.              *|
|*   - else send and error <ERR_PASSWDMISMATCH> and close the newly opened fd.                           *|
\* ----------------------------------------------------------------------------------------------------- */