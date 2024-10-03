TARGET = op
CC = gcc
CFLAGS = -gcc

SRC = main.c prompt.c Ev.c tilde.clean path.c

OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c lever.h
	$(CC) $(CLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)