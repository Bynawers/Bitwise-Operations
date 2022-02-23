all: additionneur run

run: additionneur
	./additionneur 0x1 1

additionneur: additionneur.c
	gcc additionneur.c -Wall -lm -o additionneur

clean:
	rm additionneur