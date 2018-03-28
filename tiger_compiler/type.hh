#ifndef _TYPE_HH_
#define _TYPE_HH_

#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace tiger {

enum class tiger_type {
    NOTIMPLEMENTED,
    NOTFOUND,
    ERROR,
    BASE,
    ARRAY,
    RECORD,
    FUNCTION,
};

class BaseType;

class Type {
    public:
        static BaseType *intType;
        static BaseType *stringType;
        static BaseType *nilType;

        static BaseType *errorType;
        static BaseType *notImplementedType;
        static BaseType *notFoundType;

        Type(string name);

        virtual string toStr() const =0;

    protected:
        tiger_type type_;

        string name_;
};

class ArrayType : public Type {
    public:
        ArrayType(string name, Type *what_of);

        string toStr() const;

        const Type *type_of_;
};

class RecordType : public Type {
    public:
        RecordType(string name);

        void add_field(string name, const Type* type);

        string toStr() const;

    protected:
        std::vector<std::pair<string, const Type*> > fields_;
};

class FunctionType : public Type {
    public:
        FunctionType(string name, const Type* rettype);

        void add_arg(const Type* type);

        string toStr() const;

    protected:
        const Type* rettype_;

        std::vector<const Type*> arguments_;
};

class BaseType : public Type {
    public:
        BaseType(string name);

        string toStr() const;
};

}//namespace

#endif
