TARGET = op
CC = gcc
CFLAGS = -g

SRC = main.c prompt.c Ev.c tilde.c path.c internal.c lever.h pipe.c 

OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c lever.h
	$(CC) $(CLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)
