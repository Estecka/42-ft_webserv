HDRS = \
	confparsing.hpp \
	InvalidSyntaxException.hpp \
	ServerConfig.hpp \
	ServerLocation.hpp \

SRCS = \
	confparsing.cpp \
	ServerConfig.cpp \

OBJS = ${SRCS:.cpp=.o}


TEST_SRCS = main.cpp
TEST_OBJS = ${TEST_SRCS:.cpp=.o}
TEST_LIBS = ../clibft/clibft.a


NAME = configparser.a
TEST_NAME = configparser.out

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++


all: ${NAME} ${TEST_NAME}

${NAME}: ${OBJS} ${HDRS}
	ar rcs ${NAME} ${OBJS}

test: ${TEST_NAME}
${TEST_NAME}: ${NAME} ${OBJS} ${HDRS} ${TEST_OBJS} ${TEST_LIBS}
	${CXX} ${CPPFLAGS} ${TEST_OBJS} ${NAME} ${TEST_LIBS} -o ${TEST_NAME}


%.a: submakefile
	make $(@F) -C $(@D)

headers_test: ${HDRS:.hpp=.hpp.o}
%.hpp.o: ${HDRS}
	${CXX} ${CPPFLAGS} ${@:.o=} -c -o $@
%.hpp:


clean:
	rm *.o -f

fclean: clean
	rm ${NAME}

re: fclean allrm 


.PHONY: clean fclean re all \
	submakefile \
	test \
