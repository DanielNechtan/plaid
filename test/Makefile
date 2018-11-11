CFLAGS = -I/usr/local/include -I/usr/X11R6/include -std=c99 -pedantic -Wall -Wextra -O0 -g
LDFLAGS += -L/usr/X11R6/lib -L/usr/local/lib -lc -lX11 -lXaw
XXLIBS += -lXm -lXaw -lXmu -lXt -lSM -lICE -lXext -lX11 -lm
XLIBS += -lXm -lXmu -lXt -lX11 -lpng -lz -lm -ljpeg 

DEPS = 
OBJ = plaidview.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

plaidview: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f plaidview *.o *.core
