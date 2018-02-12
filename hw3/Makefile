# uncomment to test for vectree

CXX=g++-7   # Can switch to g++ if desired
CXXFLAGS=-g -std=c++17 -Wall -pedantic -Wextra -Werror
LDFLAGS=$(CXXFLAGS)
LIBS=

INC=ptretree.hh tree.hh catch.hpp
SRC=test_ptree.cc ptretree.cc
OBJ=$(SRC:.cc=.o)

all: test_ptree

test_ptree: $(OBJ)
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $(OBJ)

%.o.cc: %.cc %.hh
	$(CXX) $(CFLAGS) -c -o $@ $<

test: all
	./test_ptree

clean:
	rm -f $(OBJ)


# CXX=g++-7   # Can switch to g++ if desired
# CXXFLAGS=-g -std=c++17 -Wall -pedantic -Wextra -Werror
# LDFLAGS=$(CXXFLAGS)
# LIBS=

# INC=vectree.hh tree.hh catch.hpp
# SRC=test_tree.cc vectree.cc
# OBJ=$(SRC:.cc=.o)

# all: test_tree

# test_tree: $(OBJ)
# 	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $(OBJ)

# %.o.cc: %.cc %.hh
# 	$(CXX) $(CFLAGS) -c -o $@ $<

# test: all
# 	./test_tree

# clean:
# 	rm -f $(OBJ)
