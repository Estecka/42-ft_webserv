HRDS = \

SRCS = \
	main.cpp \

OBJS = ${SRCS:.cpp=.o}

NAME = webserv

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++

all: ${NAME}

${NAME}: ${OBJS} ${HDRS}
	${CXX} ${CPPFLAGS} ${OBJS} -o ${NAME}

clean:
	rm *.o

fclean: clean
	rm ${NAME}

re: fclean all

.PHONY: all clean fclean re
