.PHONY: bin

all: bin/romswap

bin/romswap: romswap.c bin
	gcc -o bin/romswap romswap.c
	chmod +x bin/romswap

bin:
	mkdir -p bin

clean:
	rm -fr bin
