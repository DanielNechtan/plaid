CFLAGS = -std=c99 -pedantic -Wall -Wextra -O0 -g
LDFLAGS += -L/usr/X11R6/lib -L/usr/local/lib -lc -ltls -lssl -lcrypto 
XLIBS += -lXm -lXmu -lXt -lX11 -lpng -lz -lm -ljpeg 

plaid: plaid.o http.o
	gcc -o plaid plaid.o http.o $(CFLAGS) $(LDFLAGS)

plaid.o: plaid.c
	gcc -o plaid.o plaid.c -c $(CFLAGS)
http.o: http.c
	gcc -o http.o http.c -c $(CFLAGS)

clean:
	/bin/rm plaid *.o
