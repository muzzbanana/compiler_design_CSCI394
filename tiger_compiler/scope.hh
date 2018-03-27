#ifndef _SCOPE_HH_
#define _SCOPE_HH_

#include <map>
#include <vector>
#include "typeenum.hh"
#include "semantic.hh"

namespace tiger {

    class SymbolTable;

class Scope{
    public:
        Scope();
        void push_onto_scope();
        void pop_off_scope();
        tiger_type search(const std::string &s);

    private:
        using table_stack_ = std::vector<SymbolTable>;
        table_stack_ scope_;
};

} // namespace

#endif
