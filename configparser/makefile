HDRS = \
	confparsing.hpp \
	ServerConfig.hpp \
	ServerLocation.hpp \

SRCS = \
	confparsing.cpp \

OBJS = ${SRCS:.cpp=.o}


NAME = libconfigparser.a

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++

all: ${NAME}

${NAME}: ${OBJS} ${HDRS}
	ar rcs ${NAME} ${OBJS}

headers_test: ${HDRS:.hpp=.hpp.o}
%.hpp.o: ${HDRS}
	${CXX} ${CPPFLAGS} ${@:.o=} -c -o $@
%.hpp:

clean:
	rm *.o

fclean: clean
	rm ${NAME}

re: fclean all


.PHONY: clean fclean re all