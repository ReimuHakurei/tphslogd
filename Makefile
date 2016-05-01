all:
	@mkdir -p bin
	cc tphs-cli/tphs-cli.c libtphs/network/network.c libtphs/network/nullcbc.c -o bin/tphs-cli -std=c11

clean:
	rm bin/tphs-cli

