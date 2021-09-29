HDRS = \
	InvalidSyntaxException.hpp \
	ServerBlock.hpp \
	ServerConfig.hpp \
	UriConfig.hpp \

SRCS = \
	InvalidSyntaxException.cpp \
	ServerBlock.cpp \
	ServerConfig.cpp \
	UriConfig.cpp \

OBJS = ${SRCS:.cpp=.o}

LIBS = ../clibft/clibft.a

TEST_SRCS = main.cpp
TEST_OBJS = ${TEST_SRCS:.cpp=.o}


NAME = configparser.a
TEST_NAME = configparser.out

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++


all: ${NAME} ${TEST_NAME}

${NAME}: ${OBJS}
	ar rcs ${NAME} ${OBJS}

${OBJS}: ${HDRS} ${LIBS}
${TEST_OBJS}: ${HDRS}

test: ${TEST_NAME}
${TEST_NAME}: ${NAME} ${TEST_OBJS} ${LIBS}
	${CXX} ${CPPFLAGS} ${TEST_OBJS} ${NAME} ${LIBS} -o ${TEST_NAME}


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

re: fclean all


.PHONY: clean fclean re all \
	submakefile \
	test \
