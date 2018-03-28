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

void SymbolTable::insert_type(const string name, const Type *type) {
    types_.insert(make_pair(name, type));
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

const Type *SymbolTable::lookup_type(const string name) {
    //return symbols_[name];
    if (name == "int") {
        return Type::intType;
    } else if (name == "string") {
        return Type::stringType;
    } else if (name == "nil") {
        return Type::nilType;
    }

    auto iter = types_.find(name);
    if (iter != types_.end()) {
        return iter->second;
    } else {
        return Type::notFoundType;
    }
}

}//namespace
