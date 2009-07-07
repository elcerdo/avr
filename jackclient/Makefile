CC = gcc
TARGS = client metro
OBJS = client.o metro.o
LDFLAGS = `pkg-config --libs jack`
CFLAGS = `pkg-config --cflags jack`

client: client.o

metro: metro.o

all: client metro

clean: 
	$(RM) $(OBJS) $(TARGS)

