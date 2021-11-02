HDRS = clibft.hpp \
	ErrnoException.hpp \
	CleanExitException.hpp \
	fdstream.hpp \
	filesystem.hpp \
	misc.hpp \
	path.hpp \
	PrepackedExecve.hpp \
	string.hpp  \

SRCS = \
	ErrnoException.cpp \
	CleanExitException.cpp \
	fdstream.cpp \
	filesystem.cpp \
	path.cpp \
	PrepackedExecve.cpp \
	string.cpp  \

OBJS = ${SRCS:.cpp=.o}


NAME = clibft.a

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++

all: ${NAME}

${NAME}: ${OBJS}
	ar rcs ${NAME} ${OBJS}

${OBJS}: ${HDRS:.hpp=.hpp.o}

headers_test: ${HDRS:.hpp=.hpp.o}
%.hpp.o: ${HDRS}
	${CXX}  ${CPPFLAGS}  -c -o $@ ${@:.o=} 
%.hpp:

clean:
	rm *.o -f

fclean: clean
	rm ${NAME} -f

re: fclean all


.PHONY: clean fclean re all