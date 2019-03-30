OBJS	= main.o functions.o list.o sender_child.o receiver_child.o
SOURCE	= src/main.c src/functions.c src/list.c src/sender_child.c src/receiver_child.c
HEADER	= header-files/functions.h header-files/list.h
OUT	= mirror_client sender_child receiver_child
CC	= gcc
FLAGS	= -g3 -c -Wall

all: $(OBJS)
	$(CC) -o mirror_client main.o functions.o list.o 
	$(CC) -o sender_child sender_child.o functions.o list.o 
	$(CC) -o receiver_child receiver_child.o functions.o list.o

main.o: src/main.c
	$(CC) $(FLAGS) src/main.c

sender_child.o: src/sender_child.c
	$(CC) $(FLAGS) src/sender_child.c

receiver_child.o: src/receiver_child.c
	$(CC) $(FLAGS) src/receiver_child.c

functions.o : src/functions.c
	$(CC) $(FLAGS) src/functions.c

list.o : src/list.c
	$(CC) $(FLAGS) src/list.c

clean: 
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

