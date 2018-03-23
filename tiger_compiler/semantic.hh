#include <map>
#include "ast.hh"

using namespace tiger;

class Type {
    public:
        Type();
        Type(Type& other);

        virtual std::string toStr();
};

class FunctionType {
    public:
        FunctionType();
        FunctionType(FunctionType& other);

        virtual std::string toStr() {
              std::stringstream ss;
              for (int i = 0; i < inputs_.size(); i++) {
                  if (i != 0) ss << ", ";
                  ss << inputs_[i]->toStr();
              }
              ss << " -> ";
              ss << output->toStr();

              return ss.str();
        }

    private:
        std::vector<shared_ptr<Type> > inputs_;
        shared_ptr<Type> output;
};

class IntType {
    public:
        IntType();
        IntType(IntType& other);

        virtual std::string toStr() {
            return "int";
        }
};

class StringType {
    public:
        StringType();
        StringType(StringType& other);

        virtual std::string toStr() {
            return "string";
        }
};

class ArrayType {
    public:
        ArrayType(shared_ptr<Type> array_of);
        ArrayType(ArrayType& other);

        virtual std::string toStr() {
            return std::string("array of ") + array_of->toStr();
        }

    private:
        shared_ptr<Type> array_of;
};

class SymbolTable {
    public:
        SymbolTable();
        SymbolTable(SymbolTable *parent);

        ~SymbolTable();

        // Create a symbol table that's the same as this one but with another entry
        void insert(std::string name, Type *t);

        Type *lookup(std::string name);

    private:
        std::map<std::string, Type*> symbols_;
        SymbolTable *parent_;
};

int semantic_checks(ASTNode::ASTptr ast);
