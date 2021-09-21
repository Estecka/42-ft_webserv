HRDS = \
	HttpRequest.hpp \
	PollManager.hpp \
	IPollListener.hpp \
	RequestReadPollListener.hpp \
	Server.hpp \
	Socket.hpp \
	SocketPollListener.hpp \

SRCS = main.cpp \
	HttpRequest.cpp \
	PollManager.cpp \
	RequestReadPollListener.cpp \
	Server.cpp \
	Socket.cpp \
	SocketPollListener.cpp \

LIBS = \
	clibft/clibft.a \
	configparser/configparser.a \


OBJS = ${SRCS:.cpp=.o}

NAME = webserv

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g
CXX = clang++

all: ${NAME}

${NAME}: ${OBJS} ${HDRS} ${LIBS}
	${CXX} ${CPPFLAGS} ${OBJS} ${LIBS} -o ${NAME}


%.a: submakefile
	make $(@F) -C $(@D)

headers_test: ${HDRS:.hpp=.hpp.o}
%.hpp.o: ${HDRS}
	${CXX} ${CPPFLAGS} ${@:.o=} -c -o $@
%.hpp:


clean:
	rm *.o -f

fclean: clean
	rm ${NAME} -f

re: fclean all

.PHONY: all clean fclean re submakefile
