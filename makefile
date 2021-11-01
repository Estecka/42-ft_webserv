HDRS = \
	AutoIndex.hpp \
	CGILauncher.hpp \
	Cgi2Http.hpp \
	ErrorPage.hpp \
	GetFileData.hpp \
	HttpCode.hpp \
	HttpException.hpp \
	InputPollListener.hpp \
	IPollListener.hpp \
	Methods.hpp \
	OutputPollListener.hpp \
	PollManager.hpp \
	PostMethod.hpp \
	RequestHandler.hpp \
	ReqBodyExtractor.hpp \
	ReqHeadExtractor.hpp \
	RequestHeader.hpp \
	ResponseHeader.hpp \
	Server.hpp \
	Socket.hpp \
	SocketPollListener.hpp \
	TimeoutManager.hpp \
	includes/webserv.hpp \

SRCS = main.cpp \
	AutoIndex.cpp \
	CGILauncher.cpp \
	Cgi2Http.cpp \
	ErrorPage.cpp \
	GetFileData.cpp \
	HttpCode.cpp \
	HttpException.cpp \
	InputPollListener.cpp \
	Methods.cpp \
	OutputPollListener.cpp \
	PollManager.cpp \
	PostMethod.cpp \
	RequestHandler.cpp \
	ReqBodyExtractor.cpp \
	ReqHeadExtractor.cpp \
	RequestHeader.cpp \
	ResponseHeader.cpp \
	Server.cpp \
	Socket.cpp \
	SocketPollListener.cpp \
	TimeoutManager.cpp \

LIBS = \
	logutil/logutil.a \
	clibft/clibft.a \
	configparser/configparser.a \


OBJS = ${SRCS:.cpp=.o}

NAME = webserv

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g
CXX = clang++

all: ${NAME}

${NAME}: ${OBJS} ${LIBS}
	${CXX} ${CPPFLAGS} ${OBJS} ${LIBS} -o ${NAME}

${OBJS}: ${HDRS:.hpp=.hpp.o}


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

