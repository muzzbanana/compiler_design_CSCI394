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

Type::Type(string name) {
    name_ = name;
}

ArrayType::ArrayType(string name, Type *what_of) : Type(name) {
    type_of_ = what_of;
    type_ = tiger_type::ARRAY;
}

string ArrayType::toStr() const {
    std::stringstream ss;
    ss << "array of ";
    ss << type_of_->toStr();
    return ss.str();
}

RecordType::RecordType(string name) : Type(name) {
    type_ = tiger_type::RECORD;
}

void RecordType::add_field(string name, const Type* type) {
    this->fields_.push_back(std::make_pair(name, type));
}

std::string RecordType::toStr() const {
    return name_;
}

FunctionType::FunctionType(string name, const Type* rettype) : Type(name) {
    rettype_ = rettype;
    type_ = tiger_type::FUNCTION;
}

void FunctionType::add_arg(const Type* type) {
    arguments_.push_back(type);
}

string FunctionType::toStr() const {
    std::stringstream ss;
    ss << name_;
    ss << " (";
    for (auto a : arguments_) {
        ss << a->toStr();
    }
    ss << ") : ";
    ss << rettype_->toStr();
    return ss.str();
}

BaseType::BaseType(string name) : Type(name) {
    type_ = tiger_type::BASE;
}

string BaseType::toStr() const {
    return name_;
}

}//namespace
