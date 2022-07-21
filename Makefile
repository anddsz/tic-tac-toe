CC = gcc
CFLAGS = -Wall
SOURCES = main.c
TARGET = game
OBJECTS = $(SOURCES:.c=.o)
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY:
	clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core