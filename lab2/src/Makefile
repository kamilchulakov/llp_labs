APP=lab2

all: build run

build: clean
	bison -d parser.y -Wcounterexamples
	flex lexer.l
	cc -g -o $(APP) main.c parser.tab.c lex.yy.c ast.c -lfl

run:
	./$(APP)

clean:
	rm -f $(APP) lex.yy.c parser.tab.c parser.tab.h
