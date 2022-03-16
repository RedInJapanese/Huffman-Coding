CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
SORUCES = code.c io.c huffman.c node.c pq.c stack.c 
EXEC = test
OBJECTS = $(SOURCES)

all: encode decode

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o encode code.c io.c huffman.c node.c pq.c stack.c encode.c -g
	$(CC) $(CFLAGS) -o decode code.c io.c huffman.c node.c pq.c stack.c decode.c -g


%.o:%.c
	$(CC) $(CFLAGS) -c $<

tidy:
	rm -rf $(OBJECTS)

clean: tidy
	rm -rf encode decode

format: 
	clang-format -i -style=file *.[c,h]

encode:
	$(CC) $(CFLAGS) -o encode code.c io.c huffman.c node.c pq.c stack.c encode.c -g

decode:
	$(CC) $(CFLAGS) -o decode code.c io.c huffman.c node.c pq.c stack.c decode.c -g

