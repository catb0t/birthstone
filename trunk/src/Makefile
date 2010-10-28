CC=g++
FLAGS=-fshort-enums

default: birthstone
	chmod +x ../bin/birthstone

debug: bs-debug
	ddd ../bin/bs-debug &

bs-rl-debug: bs-rl.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) -g -DBS_DEBUG -lncurses -lreadline -lhistory token.cpp saved_token_stream.cpp lexer.cpp parser.cpp bs-rl.cpp -o ../bin/bs-rl-debug

bs-rl: bs-rl.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) -lncurses /usr/lib/libreadline.so /usr/lib/libhistory.so token.cpp saved_token_stream.cpp lexer.cpp parser.cpp bs-rl.cpp -o ../bin/bs-rl

bs-debug: birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) -g -DBS_DEBUG token.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/bs-debug

birthstone: birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) token.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/birthstone

lexer-test: lexer.cpp lexer.h token.h token.cpp
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG token.cpp lexer.cpp -o lexer

clean:
	rm -v ../bin/*.o birthstone