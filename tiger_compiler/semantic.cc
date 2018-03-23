#include "semantic.hh"

namespace tiger {

/*Type::Type(Type& other) {
}*/

SymbolTable::SymbolTable() : parent_(NULL) {
}

SymbolTable::SymbolTable(SymbolTable *parent) : parent_(parent) {
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::insert(string name, type_t type) {
    symbols_.insert(make_pair(name, type));
}

type_t SymbolTable::lookup(string name) {
    return symbols_[name];
}

int semantic_checks(ASTNode::ASTptr ast) {
}

}
