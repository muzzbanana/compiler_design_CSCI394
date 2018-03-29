#ifndef _TYPE_HH_
#define _TYPE_HH_

#include <iostream>
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
        static BaseType *incompleteRecursiveType;

        Type(string name, tiger_type type);

        virtual bool equivalent(const Type *other) const;

        virtual string toStr() const =0;

        virtual ~Type() =default;

        tiger_type getKind() const;

    protected:
        tiger_type type_;

        string name_;
};

class ArrayType : public Type {
    public:
        ArrayType(const Type *what_of);

        string toStr() const;

        bool equivalent(const Type *other) const;

        const Type *type_of_;
};

class RecordType : public Type {
    public:
        /* create blank record type */
        RecordType();

        /* create record type that is the same but with a new name */
        RecordType(string name, const RecordType *other);

        void add_field(string name, const Type* type);

        string toStr() const;

        const Type *field_type(string name) const;

        std::vector<std::pair<string, const Type*> > fields_;
};

class FunctionType : public Type {
    public:
        FunctionType(string name, const RecordType* args, const Type* rettype);

        void add_arg(const Type* type);

        string toStr() const;

        const Type* rettype_;

        const RecordType *args_;
};

class BaseType : public Type {
    public:
        BaseType(string name);

        string toStr() const;
};

}//namespace

#endif
