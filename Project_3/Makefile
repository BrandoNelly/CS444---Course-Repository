pc: pc.c eventbuf.c
	gcc -Wall -Wextra -o pc pc.c eventbuf.c -lpthread

pc.zip:
	rm -f $@
	zip $@ Makefile pc.c
