CFLAGS=-g -Wall
LDLIBS=-lfl
TARGET=algparse
OBJS= algparse-parse.tab.o algparse-lex.o
BISON=bison

%.tab.c: %.y
	$(BISON) -d $^

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(OBJS) $(TARGET)

