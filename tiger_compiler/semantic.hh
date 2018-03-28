#ifndef _SEMANTIC_HH_
#define _SEMANTIC_HH_

#include <map>
#include <vector>
#include <sstream>
#include <memory>
#include "type.hh"

using namespace std;

namespace tiger {

class ASTNode;

class SymbolTable {
    public:
        SymbolTable();

        ~SymbolTable();

        // Add a new entry to this scope.
        void insert(const string name, const Type *type);

        // Add a new type entry to this scope.
        void insert_type(const string name, const Type *type);

        const Type *lookup(string name);
        const Type *lookup_type(const string name);

    private:
        map<string, const Type*> symbols_;
        map<string, const Type*> types_;
};

}//namespace

#endif
