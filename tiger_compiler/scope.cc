#include <iostream>
#include "scope.hh"

namespace tiger{

Scope::Scope(){}

void Scope:: push_scope(){
    scope_.push_back(SymbolTable());
}

void Scope:: pop_scope(){
    scope_.pop_back();
}

/* check if it's in the top scope, to allow variable shadowing
 * but forbid declaring two things with same name in same scope */
bool Scope::preexisting(const std::string& s){
    SymbolTable top_scope = scope_[scope_.size() - 1];
    const Type *the_type = top_scope.lookup(s);
    return (the_type != Type::notFoundType);
}

/* same but with types */
bool Scope::preexisting_type(const std::string& s){
    SymbolTable top_scope = scope_[scope_.size() - 1];
    const Type *the_type = top_scope.lookup_type(s);
    return (the_type != Type::notFoundType);
}

/* search through scopes in reverse order, if symbol is not
   found, return error, otherwise return type of symbol*/
const Type *Scope::search(const std::string& s){
    for (table_stack_::reverse_iterator scope = scope_.rbegin();
        scope != scope_.rend(); scope++){
        const Type *the_type = scope->lookup(s);
        if (the_type != Type::notFoundType){
            return the_type;
        }
    }
    return Type::notFoundType;
}

/* same, but look for a type-name instead of a variable name */
const Type *Scope::type_search(const std::string& s) {
    for (table_stack_::reverse_iterator scope = scope_.rbegin();
        scope != scope_.rend(); scope++){
        const Type *the_type = scope->lookup_type(s);
        if (the_type != Type::notFoundType){
            return the_type;
        }
    }
    return Type::notFoundType;
}

void Scope::symbol_insert(const std::string &s, const Type *type) {
    scope_[scope_.size()-1].insert(s, type);
}

void Scope::type_insert(const std::string &s, const Type *type) {
    scope_[scope_.size()-1].insert_type(s, type);
}

} //namespace
