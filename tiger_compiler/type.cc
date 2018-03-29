#include "type.hh"

using namespace std;

namespace tiger {

/* declare the basic types */
BaseType *Type::intType = new BaseType("int");
BaseType *Type::stringType = new BaseType("string");
BaseType *Type::nilType = new BaseType("nil");
BaseType *Type::errorType = new BaseType("<error>");
BaseType *Type::notImplementedType = new BaseType("<not implemented>");
BaseType *Type::notFoundType = new BaseType("<not found>");

Type::Type(string name, tiger_type type) {
    name_ = name;
    type_ = type;
}

tiger_type Type::getKind() const {
    return type_;
}

bool Type::equivalent(const Type *other) const {
    return (this == other);
}

ArrayType::ArrayType(const Type *what_of) : Type("array", tiger_type::ARRAY) {
    type_of_ = what_of;
}

string ArrayType::toStr() const {
    std::stringstream ss;
    ss << "array of ";
    ss << type_of_->toStr();
    return ss.str();
}

bool ArrayType::equivalent(const Type *other) const {
    if (other->getKind() != tiger_type::ARRAY) return false;
    // I feel bad for doing this. But we know it's an array type!
    // There's probably a better C++-y way to do it, though.
    const ArrayType *o = static_cast<const ArrayType*>(other);
    return o->type_of_ == type_of_;
}

RecordType::RecordType() : Type("", tiger_type::RECORD) {
}

RecordType::RecordType(string name, const RecordType *other) : Type(name, tiger_type::RECORD) {
    for (auto a : other->fields_) {
        add_field(a.first, a.second);
    }
}

void RecordType::add_field(string name, const Type* type) {
    fields_.push_back(std::make_pair(name, type));
}

const Type *RecordType::field_type(string name) const {
    for (auto a : fields_) {
        if (a.first == name) {
            return a.second;
        }
    }
    return Type::notFoundType;
}

std::string RecordType::toStr() const {
    return name_;
}

FunctionType::FunctionType(string name, const RecordType* args, const Type* rettype) : Type(name, tiger_type::FUNCTION) {
    rettype_ = rettype;
    type_ = tiger_type::FUNCTION;
    args_ = args;
}

string FunctionType::toStr() const {
    std::stringstream ss;
    ss << name_;
    ss << " (";
    for (auto a : args_->fields_) {
        ss << a.first;
        ss << ": ";
        ss << a.second->toStr();
    }
    ss << ") : ";
    ss << rettype_->toStr();
    return ss.str();
}

BaseType::BaseType(string name) : Type(name, tiger_type::BASE) {
    type_ = tiger_type::BASE;
}

string BaseType::toStr() const {
    return name_;
}

}//namespace
