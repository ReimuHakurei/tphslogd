all:
	@mkdir -p bin
	cc tphs-cli/tphs-cli.c libtphs/network/network.c libtphs/network/nullcbc.c libs/libjansson.a -o bin/tphs-cli -std=c11

clean:
	rm bin/tphs-cli

.PHONY: depends
depends:
	@mkdir -p libs
	rm -rf depends/jansson-2.7
	tar -xjf depends/jansson-2.7.tar.bz2 -C depends/
	cd depends; cd jansson-2.7; ./configure; make;
	cp depends/jansson-2.7/src/.libs/libjansson.a libs/