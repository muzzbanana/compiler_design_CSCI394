#include "scope.hh"

namespace tiger{

Scope::Scope(){}

void Scope::push_onto_scope(){
    scope_.push_back(SymbolTable());
}

void Scope::pop_off_scope(){
    scope_.pop_back();
}

/* search through scopes in reverse order, if symbol is not
   found, return error, otherwise return type of symbol*/
tiger_type
Scope::search(const std::string& s){
    for (symbol_scope_t::reverse_iterator scope = scope_.rbegin();
        scope != scope_.rend(); scope++){
        if (tiger_type the_type = scope->lookup(s)){
            return the_type;
        }
    }
    return tiger_type::ERROR;
}

} //namespace
