all:
	@mkdir -p bin
	cc tphs-cli/tphs-cli.c libtphs/network/network.c libtphs/network/nullcbc.c libtphs/json/json.c libs/libjansson.a -o bin/tphs-cli -std=c11
	cc tphslogd/tphslogd.c libtphs/network/network.c libtphs/network/nullcbc.c libtphs/json/json.c bin/sqlite3.o libs/libjansson.a -pthread -ldl -w -o bin/tphslogd -std=c11


clean:
	rm bin/tphs-cli
	rm bin/tphslogd

.PHONY: depends
depends:
	@mkdir -p libs
	rm -rf depends/jansson-2.7
	tar -xjf depends/jansson-2.7.tar.bz2 -C depends/
	cd depends; cd jansson-2.7; ./configure; make;
	cp depends/jansson-2.7/src/.libs/libjansson.a libs/
	cc -c sqlite3/sqlite3.c -o bin/sqlite3.o