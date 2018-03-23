#include <map>
#include "ast.hh"

using namespace tiger;

class Type {
    public:
        Type(Type& other);
};

class SymbolTable {
    public:
        SymbolTable();
        SymbolTable(SymbolTable *parent);

        ~SymbolTable();

        // Create a symbol table that's the same as this one but with another entry
        void insert(std::string name, Type *t);

        Type *lookup(std::string name);

    private:
        std::map<std::string, Type*> symbols_;
        SymbolTable *parent_;
};

int semantic_checks(ASTNode::ASTptr ast);
