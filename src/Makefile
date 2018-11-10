CFLAGS = -I/usr/local/include -I/usr/X11R6/include -std=c99 -pedantic -Wall -Wextra -O0 -g
LDFLAGS += -L/usr/X11R6/lib -L/usr/local/lib -L../3rd/tidy-html5/build/cmake -lc -ltls -lssl -lcrypto -lX11 -lXaw ../3rd/tidy-html5/build/cmake/libtidys.a
XXLIBS += -lXm -lXaw -lXmu -lXt -lSM -lICE -lXext -lX11 -lm
XLIBS += -lXm -lXmu -lXt -lX11 -lpng -lz -lm -ljpeg 

plaid: plaid.o http.o util.o gui.o html.o
	gcc -o plaid plaid.o http.o util.o gui.o html.o $(CFLAGS) $(LDFLAGS)

plaid.o: plaid.c
	gcc -o plaid.o plaid.c -c $(CFLAGS)

http.o: http.c
	gcc -o http.o http.c -c $(CFLAGS)

util.o: util.c
	gcc -o util.o util.c -c $(CFLAGS)

gui.o: gui.c
	gcc -o gui.o gui.c -c $(CFLAGS)

html.o: html.c
	gcc -o html.o html.c -c $(CFLAGS)

clean:
	/bin/rm -f plaid *.o *.core
