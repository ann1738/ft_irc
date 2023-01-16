import socket
from colorama import Fore
# IRC server information
SERVER = "127.0.0.1"
PORT = 6667
PASSWORD = "pass"
NICKNAME = "nickyy"

class TestCase:
	def __init__(self, test_case, correct_response, test_description):
		self.test_case = test_case
		self.correct_response = correct_response
		self.test_description = test_description

	def test_case(self):
		return self.test_case

	def correct_response(self):
		return self.correct_response

	def test_description(self):
		return self.test_description


def check_response(correct_response, actual_response):
	return correct_response in actual_response

def test_commands(commands, correct_response, test_description):
	# create a socket and connect to the server
	irc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	irc.connect((SERVER, PORT))

	# send the commands to the server
	for command in commands:
		irc.send(bytes(command + "\r\n", "utf-8"))

	# save the server's response to a variable
	response = irc.recv(2048).decode("utf-8")

	# close the socket
	irc.close()

	print (response)

	if check_response(correct_response, response) == True:
		print(Fore.GREEN + "PASS!: " + Fore.LIGHTBLACK_EX + test_description + Fore.RESET)
	else:
		print(Fore.RED + "FAIL: " + Fore.LIGHTBLACK_EX + test_description + Fore.RESET)
	return response
 

registration_commands = "CAP LS" + "\r\n" \
					  + "PASS " + PASSWORD + "\r\n" \
			  		  +	"NICK " + NICKNAME + "\r\n" \
			 		  +	"USER " + NICKNAME + " " + NICKNAME + " " + SERVER + " :" + NICKNAME + "\r\n";
quit_command = "QUIT\r\n"

# list of commands to send to the server
test1_commands = [	registration_commands,
					"",
				 	"JOIN #a",
					"PART #a",
					quit_command];

test1_correct_rpl = ":Welcome to the Internet Relay Network"

test1_desc = "Proper Registration Test"

test1 = TestCase(test1_commands, ":Welcome to the Internet Relay Network", "Proper Registration Test")

test_commands(test1.test_case, test1.correct_response, test1.test_description)