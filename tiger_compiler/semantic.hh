#ifndef _SEMANTIC_HH_
#define _SEMANTIC_HH_

#include <map>
#include <vector>
#include <sstream>
#include <memory>
#include "typeenum.hh"

using namespace std;

namespace tiger {

    class ASTNode;

/* class Type {
    public:
        Type();
        Type(Type& other);

        virtual string toStr();
};

class FunctionType : public Type {
    public:
        FunctionType();
        FunctionType(FunctionType& other);

        virtual string toStr() {
              stringstream ss;
              for (int i = 0; i < inputs_.size(); i++) {
                  if (i != 0) ss << ", ";
                  ss << inputs_[i]->toStr();
              }
              ss << " -> ";
              ss << output->toStr();

              return ss.str();
        }

    private:
        vector<shared_ptr<Type> > inputs_;
        shared_ptr<Type> output;
};

class BaseType : public Type {
    public:
        BaseType(string repr) : repr_(repr) { }
        BaseType(BaseType& other);

        virtual string toStr() {
            return repr_;
        }

    private:
        string repr_;
};

class StringType : public Type {
    public:
        StringType();
        StringType(StringType& other);

        virtual string toStr() {
            return "string";
        }
};

class ArrayType : public Type {
    public:
        ArrayType(shared_ptr<Type> array_of);
        ArrayType(ArrayType& other);

        virtual string toStr() {
            return string("array of ") + array_of->toStr();
        }

    private:
        shared_ptr<Type> array_of;
}; */

class SymbolTable {
    public:
        SymbolTable();
        SymbolTable(SymbolTable *parent);

        ~SymbolTable();

        // Create a symbol table that's the same as this one but with another entry
        void insert(string name, tiger_type type);

        tiger_type lookup(string name);

    private:
        map<string, tiger_type> symbols_;
        SymbolTable *parent_;
};

}//namespace

#endif
