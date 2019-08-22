CFLAGS += -I ./inc

CFLAGS += $(shell pkg-config --libs glfw3)

LIBS += $(shell pkg-config --libs glfw3)

LIBS += -lGL
LIBS += -ldl

all: bin/snake

bin/snake: obj/glad.o obj/snake.o obj/apple.o obj/main.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -o $@ -c $^

obj/glad.o: src/glad.c
	$(CC) $(CFLAGS) -o $@ -c $^

obj/snake.o: src/snake.c
	$(CC) $(CFLAGS) -o $@ -c $^

obj/apple.o: src/apple.c
	$(CC) $(CFLAGS) -o $@ -c $^
