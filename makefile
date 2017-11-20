all:
	rm -rf ./bin
	mkdir ./bin
	clang bytecode_runner.c -o ./bin/bcr
