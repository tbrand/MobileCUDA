CC = gcc

C_FLAGS = -g -lnvidia-ml -lcupti -L/usr/local/cuda/extras/CUPTI/lib64

INCLUDE = -I../include -I/usr/local/cuda/include -I/usr/local/cuda/extras/CUPTI/include

OBJS =  mocu_daemon.o \
	dequeue.o     \
	proc.o        \
	cons.o        \
	connect.o     \
	stamp.o       \

DAEMON = mocu_daemon

all: $(DAEMON)

$(DAEMON): $(OBJS)
	$(CC) $(C_FLAGS) $(INCLUDE) -o $@ $(OBJS)

%.o: %.c
	$(CC) -c $*.c $(INCLUDE)

clean: 
	rm $(OBJS) $(DAEMON)