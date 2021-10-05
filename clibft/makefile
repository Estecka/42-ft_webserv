HDRS = clibft.hpp \
	fdstream.hpp \
	misc.hpp \
	path.hpp \
	string.hpp  \

SRCS = \
	fdstream.cpp \
	path.cpp \
	string.cpp  \

OBJS = ${SRCS:.cpp=.o}


NAME = clibft.a

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++

all: ${NAME}

${NAME}: ${OBJS}
	ar rcs ${NAME} ${OBJS}

${OBJS}: ${HDRS}

headers_test: ${HDRS:.hpp=.hpp.o}
%.hpp.o: ${HDRS}
	${CXX} ${CPPFLAGS} ${@:.o=} -c -o $@
%.hpp:

clean:
	rm *.o -f

fclean: clean
	rm ${NAME} -f

re: fclean all


.PHONY: clean fclean re all