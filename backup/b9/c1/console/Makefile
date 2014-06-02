CC = gcc

C_FLAGS = -g -lnvidia-ml

INCLUDE = -I../include -I/usr/local/cuda/include

OBJS =  console.o \

CONSOLE = console

all: $(CONSOLE)

$(CONSOLE): $(OBJS)
	$(CC) $(C_FLAGS) $(INCLUDE) -o $@ $(OBJS)

%.o: %.c
	$(CC) -c $*.c $(INCLUDE)

clean: 
	rm $(OBJS) $(CONSOLE)