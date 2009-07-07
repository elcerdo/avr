CC = g++
TARGS = rawmidi client
OBJS = rawmidi.o client.o
LDFLAGS = `pkg-config --libs alsa`
CFLAGS = `pkg-config --cflags alsa`
CXXFLAGS = `pkg-config --cflags alsa`

all: client rawmidi

rawmidi: rawmidi.o

client: client.o

clean: 
	$(RM) $(OBJS) $(TARGS)

