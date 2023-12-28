all:
	make clean

.PHONY: s
s:
	./bakery_server

.PHONY: c
c:
	./bakery_client

.PHONY: make
make:
	make -f Makefile.bakery

.PHONY: clean
clean:
	rm *.o bakery_server bakery_client

.PHONY: rm
rm:
	make -f Makefile.bakery clean

.PHONY: x
x:
	rpcgen -aM bakery.x
	make -f Makefile.bakery

.PHONY: mf
mf:
	cat mf.bakery > Makefile.bakery
