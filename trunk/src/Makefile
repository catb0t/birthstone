CC=g++
FLAGS=-fshort-enums

default: birthstone
	chmod +x ../bin/birthstone

debug: bs-debug
	ddd ../bin/bs-debug &

bs-debug: func.h bs-rl.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) -g -DBS_DEBUG -lncurses -lreadline -lhistory func.cpp token.cpp saved_token_stream.cpp lexer.cpp parser.cpp bs-rl.cpp -o ../bin/bs-debug

birthstone: func.h bs-rl.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) -lncurses -lreadline -lhistory func.cpp token.cpp saved_token_stream.cpp lexer.cpp parser.cpp bs-rl.cpp -o ../bin/birthstone

bs-norl-debug: func.h birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) -g -DBS_DEBUG func.cpp token.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/bs-debug

bs-norl: func.h birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) func.cpp token.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/birthstone

lexer-test: func.h lexer.cpp lexer.h token.h token.cpp
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG func.cpp token.cpp lexer.cpp -o lexer

clean:
	rm -v ../bin/*