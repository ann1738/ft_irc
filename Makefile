NAME = ircserv

CXX = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g3
REMOVE = rm -rf

SRCS_DIR = srcs
SRCS_COMMAND_DIR = srcs/commands
OBJS_DIR = objs
INCLUDES_DIR = includes
INCLUDES_COMMAND_DIR = includes/commands

CMD_SRCS = NICK.cpp TOPIC.cpp JOIN.cpp PART.cpp PRIVMSG.cpp MODE.cpp PING.cpp LIST.cpp INVITE.cpp KICK.cpp QUIT.cpp
SRCS =  main.cpp initialParse.cpp server.cpp user.cpp channel.cpp commandParse.cpp command.cpp redirectCommand.cpp reply.cpp authenticate.cpp ${CMD_SRCS}

OBJS = $(addprefix ${OBJS_DIR}/, $(SRCS:%.cpp=%.o))

red=`tput setaf 1`
yellow=`tput setaf 11`
green=`tput setaf 28`
light_blue=`tput setaf 81`
purple=`tput setaf 57`
gray=`tput setaf 241`
brown=`tput setaf 130`
reset=`tput sgr0`

ART = "\
.${yellow}               .-     ${red} _________            __           ___________       __________ ${reset}\n\
.${yellow}       ,      /      ${red} |         '.         /  \         |           '.    |          |  ${brown}       _______________n_n_       ${reset}\n\
.${yellow}     ,/  /   /  ,--. ${red} |    __     \       /    \        |     __      \   |     _____|  ${brown}  _ ,-'                    \_    ${reset}\n\
.${yellow}  ,-'/  /   /  /_,-' ${red} |   |__)    |      /      \       |    |  \\     |   |    |___    ${brown}  (_/                   ' _' o\   ${reset}\n\
.${yellow} (  /  /   /  |\`     ${red} |          /      /   /\   \      |    |__/     /   |        |   ${brown}    |                    (___) )  ${reset}\n\
.${yellow}  '/  /._.'    '._.' ${red} |    ___    \    /   /__\   \     |            /    |     ___|    ${brown}   \    |_______|    |____,.-'   ${reset}\n\
.${yellow}  |_,'               ${red} |   |___)    |  /            \    |     __     \    |    |_____   ${brown}   |    |${gray}_${brown}|${gray}_____${brown}|    |${gray}n_n_       ${reset}\n\
.\t\t       ${red} |           /  /     ____     \   |    |  \     \   |          |  ${gray}  _ ${brown}\__/         \__/    ${gray}  \_    ${reset}\n\
.\t\t       ${red} |_________,'  /____/      \____\  |____|   \_____\  |__________|  ${gray} (_d888          8888  @  @ .\   ${reset}\n\
.  ________       _________           ____           __________             ______         ${gray}y888          y88p     --  )  ${reset}\n\
. /        '.    /         \         /    \         /          '.        ,-'      \`-.      ${gray}'888b_________d88b_____,.-'   ${reset}\n\
.|     _     \  |       ___/        /      \        |     _      \      /       _    \     ${gray} Y888${reset}_${gray}|${reset}_______${gray}8888${reset}_n_n_       \n\
.|    |_)    |  |      |__         /        \       |    | \     |      |      ( \`-._/    __${gray}\"YP\"         ${gray}\"YP\"   ${reset}   \_    \n\
.|           /  |         \       /    /\    \      |    |_/     /       \_     \`'-.,    (_/                   '  ' o\   \n\
.|     _     \  |       __/      /    /__\    \     |           /       _  '-.       \     |                       -- )  \n\
.|    |_)    |  |      |___     /              \    |     .     (_,.   /  '-._)      |     \    |________|    |___,.-'   \n\
.|           /  |          \   /     ______     \   |     |\        )  \             /      |   | |      |    | |        \n\
. \________.'    \_________/  (_____/      \_____)  \____/  \`._____,'   '-._______,-'       \___/-'       \__/-'         \n"

#Manadatory Rules
all: ${NAME}

${NAME}: ${OBJS_DIR} ${OBJS}
	@echo "✨⛓️ ✨ ${purple}Linking...${gray}"
	${CXX} ${CPPFLAGS} ${OBJS} -o ${NAME}
	@echo "${reser}"
	@echo ${ART}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp 
	@printf "\033[A\033[2K\r"
	${CXX} ${CPPFLAGS} -I${INCLUDES_DIR} -I${INCLUDES_COMMAND_DIR} -c $< -o $@

${OBJS_DIR}/%.o: ${SRCS_COMMAND_DIR}/%.cpp 
	@printf "\033[A\033[2K\r"
	${CXX} ${CPPFLAGS} -I${INCLUDES_DIR} -I${INCLUDES_COMMAND_DIR} -c $< -o $@

${OBJS_DIR}/%.o: %.cpp
	@echo "✨🛀✨ ${green}Compiling... ${gray}\n"
	@printf "\033[A\033[2K\r"
	${CXX} ${CPPFLAGS} -I${INCLUDES_DIR} -I${INCLUDES_COMMAND_DIR} -c $< -o $@
	@echo "${reser}"

clean:
	@echo "✨💧✨ ${light_blue}Cleaning... ${reset}"
	@${REMOVE} ${OBJS_DIR}

fclean: clean
	@${REMOVE} ${NAME}

re: fclean all

#Utility Rules
${OBJS_DIR}:
	@mkdir -p ${OBJS_DIR}

art:
	@echo ${ART}

run: all
	./${NAME} 6667 password

irssi: rm_irssi
	docker run -it --name my-running-irssi -e TERM -u $(id -u):$(id -g) \
	--log-driver=none \
    -v ${HOME}/.irssi:/home/user/.irssi:ro \
    irssi

rm_irssi:
	@docker rm -f my-running-irssi 2> /dev/null || exit 0

client:
	docker run -it -e TERM -u $(id -u):$(id -g) \
	--log-driver=none \
    -v ${HOME}/.irssi:/home/user/.irssi:ro \
    irssi

rm_clients:
	docker rm -f `docker ps -a -q --filter ancestor=irssi` || exit 0

.PHONY: all clean fclean re
