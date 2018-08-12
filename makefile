all:
	rm -rf ./bin
	mkdir ./bin
	clang ./src/bytecode_runner.c -O0 -g -o ./bin/bcr -std=c99
