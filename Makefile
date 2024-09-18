# Rename this file `Makefile` and include it in your submission if you use it.

# Replace `encounters.c` with the name of your file, or name your file `encounters.c`.
#           vvvvvvvvvvvv
encounters: encounters.c
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -f encounters
