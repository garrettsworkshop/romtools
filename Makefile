.PHONY: bin

all: bin/romswap bin/bytesplit

bin/romswap: romswap.c util.h bin
	gcc -o bin/romswap romswap.c
	chmod +x bin/romswap

bin/bytesplit: bytesplit.c util.h bin
	gcc -o bin/bytesplit bytesplit.c
	chmod +x bin/bytesplit

bin:
	mkdir -p bin

clean:
	rm -fr bin
