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

irssi:
	docker run -it --name my-running-irssi -e TERM -u $(id -u):$(id -g) \
    -p 6667:6667 \
	--log-driver=none \
    -v $HOME/.irssi:/home/user/.irssi:ro \
    irssi

.PHONY: all clean fclean re