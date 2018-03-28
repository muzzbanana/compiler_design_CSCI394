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
    tiger_type the_type = top_scope.lookup(s);
    return (the_type != tiger_type::NOTFOUND);
}

/* search through scopes in reverse order, if symbol is not
   found, return error, otherwise return type of symbol*/
tiger_type
Scope::search(const std::string& s){
    for (table_stack_::reverse_iterator scope = scope_.rbegin();
        scope != scope_.rend(); scope++){
        tiger_type the_type = scope->lookup(s);
        if (the_type != tiger_type::NOTFOUND){
            return the_type;
        }
    }
    return tiger_type::ERROR;
}

void Scope::symbol_insert(const std::string &s, tiger_type type) {
    scope_[scope_.size()-1].insert(s, type);
}

} //namespace
