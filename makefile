clean:
	rm -f ed edgrep
compile:
	gcc -o edgrep edgrep7.c
test:
	./edgrep < test.txt
all:
	make clean
	make compile
	make test

gitstuff:
	git add .
	git commit
