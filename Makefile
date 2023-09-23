SRCS=$(wildcard src/*.cc)

OBJS=$(SRCS:.cc=.o)

all: $(OBJS)
	g++ $(OBJS) -o build/fsh

clean:
	rm src/*.o build/*