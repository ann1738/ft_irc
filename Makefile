NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS = irc.cpp utils.cpp
OBJS = $(SRCS:%.cpp=%.o)

all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

run: all
	./${NAME} 6667

client:
	${CXX} simpleClient.cpp -o cli && ./cli
	rm -f cli

irssi: rm_irssi
	docker run -it --name my-running-irssi -e TERM -u $(id -u):$(id -g) \
    -p 6667:6667 \
	--log-driver=none \
    -v ${HOME}/.irssi:/home/user/.irssi:ro \
    irssi

rm_irssi:
	@docker rm -f my-running-irssi 2> /dev/null || exit 0

.PHONY: all clean fclean re