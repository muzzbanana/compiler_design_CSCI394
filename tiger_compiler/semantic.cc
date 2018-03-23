#include "semantic.hh"

Type::Type(Type& other) {
}

SymbolTable::SymbolTable() : parent_(NULL) {
}

SymbolTable::SymbolTable(SymbolTable *parent) : parent_(parent) {
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::insert(std::string name, Type *t) {
    symbols_.insert(std::make_pair(name, t));
}

Type *SymbolTable::lookup(std::string name) {
    return symbols_[name];
}

int semantic_checks(ASTNode::ASTptr ast) {
    return 0;
}
