CFLAGS += -I ./inc

CFLAGS += $(shell pkg-config --libs glfw3)

LIBS += $(shell pkg-config --libs glfw3)

LIBS += -lGL
LIBS += -ldl

all: bin/snake

bin/snake: obj/glad.o obj/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -o $@ -c $^

obj/glad.o: src/glad.c
	$(CC) $(CFLAGS) -o $@ -c $^
