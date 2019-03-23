clean:
	rm -f ed
compile:
	gcc -o ed ed5.c -w
test:
	./ed rj.txt
all:
	make clean
	make compile
	make test

testran:
	./ed random.txt
