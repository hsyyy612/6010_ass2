# Rename this file `Makefile` and include it in your submission if you use it.

# Every .c file should have a corresponding .o file below. If you use the files `encounters.c`,
# `encounters.h`, and `main.c`, nothing needs to change here.
OBJ = main.o encounters.o
.INTERMEDIATE: $(OBJ)

encounters: $(OBJ)
	gcc -o $@ $^

%o: %c
	gcc -c -o $@ $<

.PHONY: clean
clean:
	rm -f encounters
