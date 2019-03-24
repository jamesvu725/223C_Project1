clean:
	rm -f edgrep
compile:
	gcc -o edgrep edgrep8.c
test:
	./edgrep < test.txt
all:
	make clean
	make compile
	make test

gitstuff:
	git add .
	git commit
