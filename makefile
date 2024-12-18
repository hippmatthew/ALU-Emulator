CC = clang
CFLAGS = -O3

SRCS = $(wildcard src/*.c)
BINS = $(patsubst src/%.c, bin/%.o, $(SRCS))
TARGET = alu

build: $(TARGET)

$(TARGET): $(BINS)
	$(CC) -o $@ $(CFLAGS) $(BINS)

bin/%.o: src/%.c
	$(CC) -o $@ $(CFLAGS) -c $^

clean:
	rm -f test bin/*.o