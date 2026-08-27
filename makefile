TARGET = sort_args

CFLAGS = -g -Wall

LDFLAGS = -g

SOURCES = $(shell find -name "*c")

all: $(TARGET)

OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^

%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	r, f $(TARGET $(OBJECTS)
