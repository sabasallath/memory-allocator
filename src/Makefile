CC=gcc

# uncomment to compile in 32bits mode (require gcc-*-multilib packages
# on Debian/Ubuntu)
#HOST32= -m32

CFLAGS= $(HOST32) -fpic -Wall -Werror -std=c99 -g -DMEMORY_SIZE=128000
CFLAGS+= -DDEBUG
LDFLAGS= $(HOST32)
TESTS+=test_init test_alloc test_free test_fit test_fit_comparison
PROGRAMS=memshell $(TESTS)

.PHONY: clean all

all: $(PROGRAMS)

# dépendences des binaires
$(PROGRAMS): %: mem.o common.o test_utils.o instrumentation.o

# dépendances des fichiers objects
$(patsubst %.c,%.o,$(wildcard *.c)): %.o: .%.deps

.%.deps: %.c
	$(CC) $(CPPFLAGS) -MM $< | sed -e 's/\(.*\).o: /.\1.deps \1.o: /' > $@

-include $(wildcard .*.deps)

# Compilation de libmalloc.so
libmalloc.so: malloc_stub.o mem.o common.o instrumentation.o
	$(CC) -shared -Wl,-soname,$@ $^ -o $@

test_ls: libmalloc.so
	LD_PRELOAD=./libmalloc.so ls

# nettoyage
clean:
	rm -f *.o $(PROGRAMS) libmalloc.so .*.deps

tests: test_ls
	for file in $(TESTS);do ./$$file; done
	LD_PRELOAD=./libmalloc.so ls