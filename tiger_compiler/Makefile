
CXX=g++-7   # Can switch to g++ if desired
# CXX=g++-6
CXXFLAGS=-g -Og -std=c++17 -Wall -pedantic -Wextra
#CXXFLAGS=-g -Og -std=c++17 -Wall -pedantic -Wextra -Werror
#CXXFLAGS=-O3 -std=c++17 -Wall -pedantic -Wextra -Werror
LDFLAGS=$(CXXFLAGS) -lfl
LIBS=

all: test_tiger tiger

tiger: tiger.o lex.yy.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

test_tiger: test_tiger.o tiger.o lex.yy.c tokens.hh
	$(CXX) $(LIBS) -o $@ $^ $(LDFLAGS)

lex.yy.c: tiger.l tokens.hh
	lex tiger.l

%.o.cc: %.cc %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lex.yy.o: lex.yy.c
	$(CXX) $(CXXFLAGS) -c -o lex.yy.o lex.yy.c

%.o.c: %.cc %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test: all
	./test_tiger

clean:
	rm -f *.o tiger test_tiger lex.yy.c
