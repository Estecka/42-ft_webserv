HDRS = logutil.hpp \
	logcolors.hpp \
	logger.hpp \
	loglabel.hpp \
	loglevel.hpp \

SRCS = \
	logger.cpp \
	loglabel.cpp \
	loglevel.cpp \

OBJS = ${SRCS:.cpp=.o}


NAME = logutil.a
TEST_NAME = logutil.out

TEST_SRCS = main.cpp
TEST_OBJS = ${TEST_SRCS:.cpp=.o}

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = clang++

all: ${NAME} ${TEST_NAME}

${NAME}: ${OBJS}
	ar rcs ${NAME} ${OBJS}

test: ${TEST_NAME}
${TEST_NAME}: ${NAME} ${TEST_OBJS}
	${CXX} ${CPPFLAGS} ${TEST_OBJS} ${NAME} ${LIBS} -o ${TEST_NAME}

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


.PHONY: clean fclean re all test