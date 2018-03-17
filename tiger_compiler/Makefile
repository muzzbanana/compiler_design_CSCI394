
CXX=g++-7   # Can switch to g++ if desired
# # CXX=g++-6
CXXFLAGS=-g -Og -std=c++17 -Wall -pedantic -Wextra
#CXXFLAGS=-g -Og -std=c++17 -Wall -pedantic -Wextra -Werror
#CXXFLAGS=-O3 -std=c++17 -Wall -pedantic -Wextra -Werror
LDFLAGS=$(CXXFLAGS) -lfl
LIBS=


all: test_tiger_lex test_tiger_parse tiger

lex: test_tiger_lex
	./test_tiger_lex

parse: test_tiger_parse
	./test_tiger_parse

tiger: tiger.o tiger.tab.o tiger.tab.h lex.yy.o ast.hh
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

test_tiger_lex: tiger.tab.o lex.yy.o test_tiger_lex.o tiger.o tokens.hh
	$(CXX) $(LIBS) -o $@ $^ $(LDFLAGS)

test_tiger_parse: tiger.tab.o lex.yy.o test_tiger_parse.o tiger.o tokens.hh
	$(CXX) $(LIBS) -o $@ $^ $(LDFLAGS)

tiger.tab.c tiger.tab.h: tiger.y
	bison -d tiger.y

lex.yy.c: tiger.l tokens.hh
	flex tiger.l

%.o.cc: %.cc %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lex.yy.o: lex.yy.c
	$(CXX) $(CXXFLAGS) -c -o lex.yy.o lex.yy.c

tiger.tab.o: tiger.tab.c
	$(CXX) $(CXXFLAGS) -c -o tiger.tab.o tiger.tab.c

%.o.c: %.cc %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test: all
	./test_tiger_lex
	./test_tiger_parse

clean:
	rm -f *.o tiger test_tiger lex.yy.c tiger.tab.c tiger.tab.h
