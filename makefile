all:
	rm -rf ./bin
	mkdir ./bin
	clang ./src/bytecode_runner.c -o ./bin/bcr
