#include "semantic.hh"
#include <map>
#include <vector>

namespace tiger {

class Scope{
    public:
        Scope();
        void push_onto_scope();
        void pop_off_scope();
        SymbolTable search(const std::string &s);

    private:
        using symbol_scope_t = std::vector<SymbolTable>;
        symbol_scope_t scope_;


};
} // namespace
