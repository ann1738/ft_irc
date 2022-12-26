NAME = ircserv

CXX = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g3
REMOVE = rm -rf

SRCS_DIR = srcs
OBJS_DIR = objs
INCLUDES_DIR = includes

SRCS =  main.cpp initialParse.cpp server.cpp user.cpp NICK.cpp
OBJS = $(addprefix ${OBJS_DIR}/, $(SRCS:%.cpp=%.o))

#Manadatory Rules
all: ${NAME}

${NAME}: ${OBJS_DIR} ${OBJS}
	@echo "Linking..."
	${CXX} ${CPPFLAGS} ${OBJS} -o ${NAME}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp 
	${CXX} ${CPPFLAGS} -I${INCLUDES_DIR} -c $< -o $@

${OBJS_DIR}/%.o: %.cpp
	${CXX} ${CPPFLAGS} -I${INCLUDES_DIR} -c $< -o $@

clean:
	@echo "Cleaning..."
	@${REMOVE} ${OBJS_DIR}

fclean: clean
	@${REMOVE} ${NAME}

re: fclean all

#Utility Rules
${OBJS_DIR}:
	@mkdir -p ${OBJS_DIR}

run: all
	./${NAME} 6667 password

client:
	${CXX} simpleClient.cpp -o cli && ./cli
	rm -f cli

irssi: rm_irssi
	docker run -it --name my-running-irssi -e TERM -u $(id -u):$(id -g) \
	--log-driver=none \
    -v ${HOME}/.irssi:/home/user/.irssi:ro \
    irssi

rm_irssi:
	@docker rm -f my-running-irssi 2> /dev/null || exit 0


.PHONY: all clean fclean re
