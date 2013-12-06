CC = gcc
NOWARN = -Wno-parentheses -Wno-sign-compare -Wno-unused-result
CFLAGS = -std=c89 -O4 -Wall -Werror -Wextra -pedantic $(NOWARN)
LDFLAGS = -lm -lGL -lGLU -lglut
IDIRS = include
MODULES = cam kbd levelio light rain rng road terrain texture tree util
MODULES_BIN = $(patsubst %, bin/%.o, $(MODULES))
MODULES_BIN_PROF = $(patsubst %, bin/prof/%.o, $(MODULES))
MODULES_BIN_MF = $(patsubst %, bin/mf/%.o, $(MODULES));

all: main

prof: src/main.c $(MODULES_BIN_PROF)
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -pg -o $@ $^

mf: src/main.c $(MODULES_BIN_MF)
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -fmudflap -lmudflap -o $@ $^

bin/prof/%.o: src/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -pg -c -o $@ $<

bin/mf/%.o: src/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -fmudflap -lmudflap -c -o $@ $<

main: src/main.c $(MODULES_BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -o $@ $^

bin/%.o: src/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -c -o $@ $<

install:
	@echo "make: install not supported"

clean:
	rm -f main prof mf gmon.out $(MODULES_BIN) $(MODULES_BIN_PROF) $(MODULES_BIN_MF)
