#include "semantic.hh"

namespace tiger {

/*Type::Type(Type& other) {
}*/

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::insert(const string name, const Type *type) {
    symbols_.insert(make_pair(name, type));
}

const Type *SymbolTable::lookup(string name) {
    //return symbols_[name];
    auto iter = symbols_.find(name);
    if (iter != symbols_.end()) {
        return iter->second;
    } else {
        return Type::notFoundType;
    }
}

}//namespace
