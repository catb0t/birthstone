CC=g++
FLAGS=-fshort-enums

default: birthstone
	chmod +x ../bin/birthstone

debug: bs-debug
	ddd ../bin/bs-debug &

bs-debug: bs-rl.cpp parser.h lexer.h symbol.h token_stream.h token.h type_exception.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp func.h func.cpp
	$(CC) $(FLAGS) -g -DBS_DEBUG -lncurses -lreadline -lhistory token.cpp func.cpp saved_token_stream.cpp lexer.cpp parser.cpp bs-rl.cpp -o ../bin/bs-debug

birthstone: bs-rl.cpp parser.h lexer.h symbol.h token_stream.h token.h type_exception.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp func.h func.cpp
	$(CC) $(FLAGS) -lncurses -lreadline -lhistory token.cpp func.cpp saved_token_stream.cpp lexer.cpp parser.cpp bs-rl.cpp -o ../bin/birthstone

bs-norl-debug: birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h type_exception.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp func.h func.cpp
	$(CC) $(FLAGS) -g -DBS_DEBUG token.cpp func.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/bs-debug

bs-norl: birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h type_exception.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp func.h func.cpp
	$(CC) $(FLAGS) token.cpp func.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/birthstone

lexer-test: lexer.cpp lexer.h token.h type_exception.h token.cpp func.h func.cpp
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG token.cpp func.cpp lexer.cpp -o lexer

clean:
	rm -v ../bin/*