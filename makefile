HRDS = \
	CGILauncher.hpp \
	HttpCode.hpp \
	HttpHeader.hpp \
	HttpRequest.hpp \
	PollManager.hpp \
	IPollListener.hpp \
	Server.hpp \
	ServerDispatchPollListener.hpp \
	Socket.hpp \
	SocketPollListener.hpp \
	ErrorPage.hpp \
	includes/webserv.hpp 

SRCS = main.cpp \
	CGILauncher.cpp \
	HttpCode.cpp \
	HttpHeader.cpp \
	HttpRequest.cpp \
	PollManager.cpp \
	Server.cpp \
	ServerDispatchPollListener.cpp \
	Socket.cpp \
	SocketPollListener.cpp \
	ErrorPage.cpp

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

${OBJS}: ${HDRS}


%.a: submakefile
	make $(@F) -C $(@D)

headers_test: ${HDRS:.hpp=.hpp.o}
%.hpp.o: ${HDRS}
	${CXX} ${CPPFLAGS} ${@:.o=} -c -o $@
%.hpp:


clean:
	rm *.o -f
	@for lib in ${LIBS}; do \
		make clean -C $$(dirname $$lib);\
	done;

fclean: clean
	rm ${NAME} -f
	@for lib in ${LIBS}; do \
		make fclean -C $$(dirname $$lib);\
	done;

re: fclean all

.PHONY: all clean fclean re submakefile
