CC=gcc
LIBS=liblo
CFLAGS=`pkg-config --cflags $(LIBS)` -Wall
LDFLAGS=`pkg-config --libs $(LIBS)`

all: client

clean:
	$(RM) client *.o

client: client.o

