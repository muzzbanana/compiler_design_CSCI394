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

void SymbolTable::insert(string name, tiger_type type) {
    symbols_.insert(make_pair(name, type));
}

tiger_type SymbolTable::lookup(string name) {
    return symbols_[name];
}

}//namespace
