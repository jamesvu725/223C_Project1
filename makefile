clean:
	rm -f ed edgrep
compile:
	gcc -o edgrep edgrep2.c
test:
	./edgrep < test.txt
all:
	make clean
	make compile
	make test
