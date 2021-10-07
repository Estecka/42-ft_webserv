CPPFLAGS = -Wall -Wextra
CXX = clang++

LIBS= ../clibft/clibft.a

all: pseudo_cgi.out filedesc_server.out filestream_server.out


%.out: ${@:.out=.main.cpp} ${LIBS}
	${CXX} ${CPPFLAGS} ${@:.out=.main.cpp} ${LIBS} -o ${@}

%.a: submakefile
	make ${@F} -C ${@D}

clean:
	rm -f *.o

fclean:
	rm -f *.out

re: fclean all

.PHONY: all clean fclean re submakefile
