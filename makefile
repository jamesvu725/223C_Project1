clean:
	rm -f edgrep
compile:
	gcc -o edgrep edgrep10.c
test:
	./edgrep < test.txt
all:
	make clean
	make compile
	make test

gitstuff:
	git add .
	git commit
