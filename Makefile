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

.PHONY: all clean fclean re