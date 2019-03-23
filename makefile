clean:
	rm -f ed
compile:
	gcc -o ed edgrep.c
test:
	./ed < test.txt
all:
	make clean
	make compile
	make test
