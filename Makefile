CFLAGS += -std=gnu99
LIBS += -lgc

SOURCES = bool.c  caller.c  church_numerals.c  list.c  main.c  onerr.c  pair.c  xgc.c

.PHONY: all clean

all: demo

demo: ${SOURCES:.c=.o}
	${CC} ${CFLAGS} ${LDFLAGS} $^ ${LIBS} -o demo

clean:
	rm -f demo ${SOURCES:.c=.o}
