CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm

gen_tables: gen_tables.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

verify: gen_tables
	./gen_tables

emit: gen_tables
	./gen_tables --emit

clean:
	rm -f gen_tables gen_tables.exe

.PHONY: verify emit clean
