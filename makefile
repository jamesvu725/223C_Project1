clean:
	rm -f ed
compile:
	gcc -o ed ed2.c -w
test:
	./ed rj.txt
all:
	make clean
	make compile
	make test
gitstuff:
	git status
	git add .
	git commit
