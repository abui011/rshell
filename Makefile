all:
		mkdir -p ./bin
		g++ -Wall  -pedantic -ansi  ./src/assn4.cpp -o ./bin/rshell
rshell:
		mkdir -p ./bin
		g++ -Wall  -pedantic -ansi  ./src/assn4.cpp -o ./bin/rshell
clean:
		rm -rf ./bin
