HRDS = \
	HttpRequest.hpp \
	Server.hpp \
	Socket.hpp \

SRCS = main.cpp \
	HttpRequest.cpp \
	Server.cpp \
	Socket.cpp \

LIBS = \
	clibft/clibft.a \
	configparser/configparser.a \


OBJS = ${SRCS:.cpp=.o}

NAME = webserv

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
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
	rm *.o

fclean: clean
	rm ${NAME}

re: fclean all

.PHONY: all clean fclean re submakefile
