OBJS	= main.o functions.o list.o
SOURCE	= src/main.c src/functions.c src/list.c
HEADER	= header-files/functions.h header-files/list.h
OUT	= mirror_client
CC	= gcc
FLAGS	= -g3 -c -Wall

$(OUT):	$(OBJS)
	$(CC) -g3 $(OBJS) -o $@

main.o: src/main.c
	$(CC) $(FLAGS) src/main.c

functions.o : src/functions.c
	$(CC) $(FLAGS) src/functions.c

list.o : src/list.c
	$(CC) $(FLAGS) src/list.c

clean: 
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

