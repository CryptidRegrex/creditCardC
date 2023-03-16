COPTS=-Wall -pedantic -std=c1x

tracktwo :	tracktwo.c
	gcc $(COPTS) -o tracktwo tracktwo.c

clean :
	rm *~ ./tracktwo