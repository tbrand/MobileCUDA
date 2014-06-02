CC = gcc

C_FLAGS = -g -shared -fPIC -lpthread -ldl -lstdc++

INCLUDE = -I../include -I/usr/local/cuda/include

OBJS =  device.o    \
	event.o     \
	libcudart.o \
	load.o      \
	migration.o \
	replay.o    \
	stream.o    \
	signal.o    \
	socket.o    \
	parse.o     \
	symbol.o    \

SHARED = libcudart.so
LINK1  = libcudart.so.5.5
LINK2  = libcudart.so.5.5.22

all: $(SHARED)

$(SHARED): $(OBJS)
	$(CC) $(C_FLAGS) $(INCLUDE) -o $@ $(OBJS)
	cp -f $(SHARED) ../lib64

%.o: %.c
	$(CC) -fPIC -c $*.c $(INCLUDE)

link:
	ln -s ../lib64/$(SHARED) ../lib64/$(LINK1)
	ln -s ../lib64/$(SHARED) ../lib64/$(LINK2)

clean: 
	rm $(OBJS) $(SHARED)