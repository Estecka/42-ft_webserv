CPPFLAGS = -Wall -Wextra
CXX = clang++

all: pseudo_cgi.out pseudo_server.out

pseudo_cgi.out: main.pseudo_cgi.cpp
	${CXX} ${CPPFLAGS} main.pseudo_cgi.cpp -o pseudo_cgi.out

pseudo_server.out: main.pseudo_server.cpp
	${CXX} ${CPPFLAGS} main.pseudo_server.cpp -o pseudo_server.out

clean:
	rm -f *.o

fclean:
	rm -f *.out

re: fclean all

.PHONY: all clean fclean re
