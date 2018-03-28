#ifndef _AST_HH_
#define _AST_HH_


#include <iostream>
#include <sstream>
#include <functional>
#include <string>
#include <cmath>
#include <set>
#include "typeenum.hh"
#include "scope.hh"

using namespace std;

/* interface to the lexer */
extern void error_reporting();

namespace tiger {

///////////////////////////////////////////////////////////////////////////////
// Base AST node class, to define the hierarchy.
class ASTNode {
    public:
        using value_t = double;  // All values will be floating-point
        using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

        ASTNode() = default;
        virtual ~ASTNode() = default;
        virtual tiger_type type_verify(Scope* scope) const = 0; // Determine type of expression
        virtual value_t eval() const = 0;  // Evaluate expression tree
        virtual std::string toStr() const = 0; // For printing purposes
};

///////////////////////////////////////////////////////////////////////////////
// A node type that evaluates to a single value, NIL
class NilASTNode : public ASTNode {
    public:
        NilASTNode()
            : ASTNode()
        {}
        virtual ~NilASTNode() = default;

        virtual tiger_type type_verify(Scope* scope) const {
            return tiger_type::NIL;
        }

        virtual value_t eval() const
        {
            return -1; /// FIX ME
        }

        virtual std::string toStr() const
        {
            return "nil";
        }
};

///////////////////////////////////////////////////////////////////////////////
// A node type representing a "break" statement.
class BreakASTNode : public ASTNode {
    public:
        BreakASTNode()
            : ASTNode()
        {}
        virtual ~BreakASTNode() = default;

        virtual tiger_type type_verify(Scope* scope) const {
            std::cout << "break not implemented yet!" << std::endl;
            return tiger_type::NOTIMPLEMENTED;
        }

        virtual value_t eval() const
        {
            return -1; /// FIX ME
        }

        virtual std::string toStr() const
        {
            return "break";
        }
};

///////////////////////////////////////////////////////////////////////////////
// A node type that evaluates to a numeric constant:
class NumASTNode : public ASTNode {
    public:
        NumASTNode(value_t value)
            : ASTNode(), value_(value)
        {}
        virtual ~NumASTNode() = default;

        virtual tiger_type type_verify(Scope* scope) const {
            return tiger_type::INT;
        }

        virtual value_t eval() const
        {
            return value_;
        }

        virtual std::string toStr() const
        {
            return std::to_string(value_);
        }


    private:
        const value_t value_;
};


///////////////////////////////////////////////////////////////////////////////
// A node type that evaluates to a string constant:
class StrASTNode : public ASTNode {
    public:
        StrASTNode(std::string value)
            : ASTNode(), value_(value)
        {}
        virtual ~StrASTNode() = default;

        virtual tiger_type type_verify(Scope* scope) const {
            return tiger_type::STRING;
        }

        virtual std::string toStr() const
        {
            return std::string("\"") + value_ + "\"";
        }

        virtual value_t eval() const
        {
            return -1; /// FIX ME
        }

    private:
        const std::string value_;
};


///////////////////////////////////////////////////////////////////////////////
// A node type that represents a name
class NameASTNode : public ASTNode {
    public:
        NameASTNode(std::string value)
            : ASTNode(), value_(value)
        {}
        virtual ~NameASTNode() = default;

        value_t eval() const {
            return -1; /// FIX ME
        }

        virtual tiger_type type_verify(Scope* scope) const {
            return scope->search(value_);
        }

        virtual std::string toStr() const
        {
            return value_;
        }


    private:
        const std::string value_;
};


///////////////////////////////////////////////////////////////////////////////
// A node type for a unary operator (e.g., generalizing  AbsASTNode).
// It's templated by another class O for the operator. Any instance of O has an
// operator() that takes a value of type value_t and returns another value of
// type value_t.
// See for example: http://en.cppreference.com/w/cpp/utility/functional/negate
template <template <typename> class O>
    class UnaryASTNode : public ASTNode {
        public:
            // Take a single child to evalute, and a string representation of the node:
            UnaryASTNode(std::string rep, ASTptr child)
                : ASTNode(), rep_(rep), child_(child)
            {}

            virtual ~UnaryASTNode()
            {
                delete child_;
            }

            virtual tiger_type type_verify(Scope* scope) const {
                std::cout << "unary not implemented yet!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(child_->eval());
            }

            virtual std::string toStr() const
            {
                return rep_ + child_->toStr();
            }

        private:
            const std::string rep_;  // String representation of node
            const ASTptr child_;
    };

// Example usage of UnaryASTNode:
using UnaryMinusASTNode = UnaryASTNode<std::negate>;

//////////////////////////////////////////////////////////////////////////
// A unary AST node that doesn't evaluate its argument when calling eval.
template <template <typename> class O>
    class NoEvalUnaryASTNode : public ASTNode {
        public:
            // Take a single child to evalute, and a string representation of the node:
            NoEvalUnaryASTNode(std::string rep, ASTptr child)
                : ASTNode(), rep1_(rep), rep2_(""), child_(child)
            {}

            NoEvalUnaryASTNode(std::string rep1, std::string rep2, ASTptr child)
                : ASTNode(), rep1_(rep1), rep2_(rep2), child_(child)
            {}

            virtual ~NoEvalUnaryASTNode()
            {
                delete child_;
            }

            virtual tiger_type type_verify(Scope* scope) const {
                auto op = O<value_t>();
                return op.type_verify(scope, child_);
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(child_);
            }

            virtual std::string toStr() const
            {
                return rep1_ + child_->toStr() + rep2_;
            }

        private:
            const std::string rep1_, rep2_;  // String representation of node
            const ASTptr child_;
    };

///////////////////////////////////////////////////////////////////////////////
// A node type for binary operators.
// It's templated by another class O for the operator. Any instance of O has an
// operator() that takes two values of type value_t and returns another value
// of type value_t.
// See for example: http://en.cppreference.com/w/cpp/utility/functional/plus
template <template <typename> class O>
    class BinaryASTNode : public ASTNode {
        public:
            BinaryASTNode(std::string rep, ASTptr left, ASTptr right)
                : ASTNode(), rep1_(""), rep2_(rep), left_(left), right_(right)
            {}

            BinaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr right)
                : ASTNode(), rep1_(rep1), rep2_(rep2), left_(left), right_(right)
            {}

            virtual ~BinaryASTNode()
            {
                delete left_;
                delete right_;
            }

            virtual tiger_type type_verify(Scope* scope) const {
                if (left_->type_verify(scope) == tiger_type::INT
                        && right_->type_verify(scope) == tiger_type::INT) {
                    return tiger_type::INT;
                } else {
                    error_reporting();
                    cerr << "       Attempting binary operation on between 1 or more non-integer values" << endl;
                    return tiger_type::ERROR;
                }
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(left_->eval(), right_->eval());
            }

            virtual std::string toStr() const
            {
                if (rep1_.length() == 0) {
                    return  "(" + left_->toStr() +
                        " " + rep2_ + " " +
                        right_->toStr() + ")";
                } else {
                    return  "(" + rep1_ + " " + left_->toStr() +
                        " " + rep2_ + " " +
                        right_->toStr() + ")";
                }
            }

        private:
            const std::string rep1_, rep2_;  // String representation of node
            const ASTptr left_, right_;
    };


using PlusASTNode = BinaryASTNode<std::plus>;
using MinusASTNode = BinaryASTNode<std::minus>;
using MultASTNode = BinaryASTNode<std::multiplies>;
using DivASTNode = BinaryASTNode<std::divides>;
using EqualASTNode = BinaryASTNode<std::equal_to>;
using NotEqualASTNode = BinaryASTNode<std::not_equal_to>;
using GreaterASTNode = BinaryASTNode<std::greater>;
using LessASTNode = BinaryASTNode<std::less>;
using LessEqualASTNode = BinaryASTNode<std::less_equal>;
using GreaterEqualASTNode = BinaryASTNode<std::greater_equal>;
using LogicalAndASTNode = BinaryASTNode<std::logical_and>;
using LogicalOrASTNode = BinaryASTNode<std::logical_or>;


///////////////////////////////////////////////////////////////////////////////
// A node type for things that have two children but don't evaluate before
// they get called (e.g. while loops)
// It's templated by another class O for the operator. Any instance of O has an
// operator() that takes two values of type ASTptr and returns a value
// of type value_t.
// See for example: http://en.cppreference.com/w/cpp/utility/functional/plus
template <template <typename> class O>
    class NoEvalBinaryASTNode : public ASTNode {
        public:
            // parens flag = whether to surround with parens and insert spaces around operators
            NoEvalBinaryASTNode(std::string rep, ASTptr left, ASTptr right, bool parens=true)
                : ASTNode(), rep1_(""), rep2_(rep), left_(left), right_(right), parens_(parens)
            {}

            NoEvalBinaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr right, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), left_(left), right_(right), parens_(parens)
            {}

            NoEvalBinaryASTNode(std::string rep1, std::string rep2, std::string rep3, ASTptr left, ASTptr right, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), left_(left), right_(right), parens_(parens)
            {}

            virtual ~NoEvalBinaryASTNode()
            {
                delete left_;
                delete right_;
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(left_, right_);
            }

            virtual tiger_type type_verify(Scope* scope) const {
                auto op = O<value_t>();
                return op.type_verify(scope, left_, right_);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (parens_) {
                    ss << "(";
                }
                if (rep1_.length() != 0) {
                    ss << rep1_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << left_->toStr();
                if (parens_) {
                    ss << " ";
                }
                if (rep2_.length() != 0) {
                    ss << rep2_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << right_->toStr();
                if (rep3_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep3_;
                }
                if (parens_) {
                    ss << ")";
                }
                return ss.str();
            }

        private:
            const std::string rep1_, rep2_, rep3_;  // String representation of node (rep3 goes at end)
            const ASTptr left_, right_;
            const bool parens_;
    };

//////////////////////////////////////////////////////////////////////////////
// A node type for tertiary operators.
// (i.e. if-then-else)
template <template <typename> class O>
    class TertiaryASTNode : public ASTNode {
        public:
            TertiaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4,
                    ASTptr left, ASTptr middle, ASTptr right, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), left_(left), middle_(middle), right_(right), parens_(parens)
            {}

            TertiaryASTNode(std::string rep1, std::string rep2, std::string rep3,
                    ASTptr left, ASTptr middle, ASTptr right, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(""), left_(left), middle_(middle), right_(right), parens_(parens)
            {}

            TertiaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr middle, ASTptr right, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(""), rep4_(""), left_(left), middle_(middle), right_(right), parens_(parens)
            {}

            virtual ~TertiaryASTNode()
            {
                delete left_;
                delete middle_;
                delete right_;
            }

            virtual tiger_type type_verify(Scope* scope) const {
                auto op = O<value_t>();
                return op.type_verify(scope, left_, middle_, right_);
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(left_, middle_, right_);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (parens_) {
                    ss << "(";
                }
                if (rep1_.length() != 0) {
                    ss << rep1_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << left_->toStr();
                if (parens_) {
                    ss << " ";
                }
                if (rep2_.length() != 0) {
                    ss << rep2_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << middle_->toStr();
                if (rep3_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep3_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << right_->toStr();
                if (rep4_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep4_;
                }
                if (parens_) {
                    ss << ")";
                }
                return ss.str();
            }

        private:
            const std::string rep1_, rep2_, rep3_, rep4_;  // String representation of node
            const ASTptr left_, middle_, right_;
            bool parens_;
    };


//////////////////////////////////////////////////////////////////////////////
// A node type for tertiary operators.
// (i.e. if-then-else)
template <template <typename> class O>
    class QuaternaryASTNode : public ASTNode {
        public:
            QuaternaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4,
                    ASTptr one, ASTptr two, ASTptr three, ASTptr four, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), rep5_(""),
                one_(one), two_(two), three_(three), four_(four), parens_(parens)
        {}

            QuaternaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4, std::string rep5,
                    ASTptr one, ASTptr two, ASTptr three, ASTptr four, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), rep5_(rep5),
                one_(one), two_(two), three_(three), four_(four), parens_(parens)
        {}

            virtual ~QuaternaryASTNode()
            {
                delete one_;
                delete two_;
                delete three_;
                delete four_;
            }

            virtual tiger_type type_verify(Scope* scope) const {
                auto op = O<value_t>();
                return op.type_verify(scope, one_, two_, three_, four_);
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(one_, two_, three_, four_);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (parens_) {
                    ss << "(";
                }
                if (rep1_.length() != 0) {
                    ss << rep1_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << one_->toStr();
                if (rep2_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep2_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << two_->toStr();
                if (rep3_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep3_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << three_->toStr();
                if (rep4_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep4_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << four_->toStr();
                if (rep5_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep5_;
                }
                if (parens_) {
                    ss << ")";
                }
                return ss.str();
                return  "(" + rep1_ +
                    " " + one_->toStr() +
                    " " + rep2_ +
                    " " + two_->toStr() +
                    " " + rep3_ +
                    " " + three_->toStr() +
                    " " + rep4_ +
                    " " + four_->toStr() + ")";
            }

        private:
            const std::string rep1_, rep2_, rep3_, rep4_, rep5_;  // String representation of node
            const ASTptr one_, two_, three_, four_;
            bool parens_; // whether to surround with parentheses and put spaces around the 'rep' strings
    };


//////////////////////////////////////////////////////////////////////////////
// A node type for vectors of AST nodes.
// (e.g. declaration lists, parameter lists, statement sequences, ....)
template <template <typename> class O, class E> // E is the elements of the vector (?)
    class VectorASTNode : public ASTNode {
        public:
            using VASTptr = const VectorASTNode*; // Can't use smart ptr in union :(

            VectorASTNode(std::string separator)
                : ASTNode(), sep_(separator), first_(""), last_("")
            {}

            VectorASTNode(std::string separator, std::string first, std::string last)
                : ASTNode(), sep_(separator), first_(first), last_(last)
            {}

            virtual ~VectorASTNode()
            {
                for (auto a : vec_) {
                    delete a;
                }
            }

            virtual tiger_type type_verify(Scope* scope) const {
                auto op = O<value_t>();
                return op.type_verify(scope, vec_);
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(vec_);
            }

            void add_node(const E* node) {
                vec_.push_back(node);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (first_.length() != 0) {
                    ss << first_;
                }
                for (unsigned int i = 0; i < vec_.size(); i++) {
                    if (i != 0) {
                        ss << sep_;
                    }
                    ss << vec_[i]->toStr();
                }
                if (last_.length() != 0) {
                    ss << last_;
                }
                return ss.str();
            }

        private:
            std::vector<const E*> vec_;
            const std::string sep_, first_, last_;
    };

template <typename Z>
    class Assignment {
        public:
            // TODO implement variable assignment
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                tiger_type name_type = left_->type_verify(scope);
                tiger_type value_type = right_->type_verify(scope);
                if (name_type == value_type) {
                    return value_type;
                } else if (name_type != tiger_type::ERROR && value_type != tiger_type::ERROR) {
                    error_reporting();
                    cerr << "       assignment of variable to wrong type" << endl;
                    return tiger_type::ERROR;
                } else {
                    return tiger_type::ERROR;
                }
            }
    };

using AssignASTNode = NoEvalBinaryASTNode<Assignment>;

template <typename Z>
    class IfThenElse {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                if (left_->eval()) {
                    return middle_->eval();
                } else if (right_ != NULL) {
                    return right_->eval();
                } else {
                    return -1; /// FIX ME
                }
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                tiger_type cond_type = left_->type_verify(scope);
                tiger_type then_type = middle_->type_verify(scope);
                tiger_type else_type = right_->type_verify(scope);
                if (cond_type == tiger_type::INT && then_type == else_type && then_type != tiger_type::ERROR) {
                    return then_type;
                } else if (cond_type != tiger_type::INT) {
                    error_reporting();
                    cerr << "       condition in 'if' statement must evaluate to integer" << endl;
                    return tiger_type::ERROR;
                } else if (then_type != else_type) {
                    error_reporting();
                    cerr << "       true and false condition expressions in 'if' statement must have the same type" << endl;
                    return tiger_type::ERROR;
                } else {
                    return tiger_type::ERROR;
                }
            }
    };

using ConditionalASTNode = TertiaryASTNode<IfThenElse>;

template <typename Z>
    class WhileDo {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                Z result = -1; // WHAT SHOULD IT RETURN
                while (left_->eval()) {
                    result = right_->eval();
                }
                return result;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                tiger_type cond_type = left_->type_verify(scope);
                tiger_type do_type = right_->type_verify(scope);

                if (cond_type == tiger_type::INT && do_type != tiger_type::ERROR) {
                    return tiger_type::NIL;
                } else if (cond_type != tiger_type::INT) {
                    error_reporting();
                    cerr << "       condition of 'while' loop must evaluate to integer" << endl;
                    return tiger_type::ERROR;
                } else {
                    return tiger_type::ERROR;
                }
            }
    };

using WhileLoopASTNode = NoEvalBinaryASTNode<WhileDo>;

template <typename Z>
    class ForTo {
        public:
            // TODO this is not actually right, we don't have binding yet?
            Z operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
                tiger_type first_type = two_->type_verify(scope);
                tiger_type last_type = three_->type_verify(scope);
                tiger_type body_type = four_->type_verify(scope);
                if (first_type == tiger_type::INT && last_type == tiger_type::INT
                        && body_type != tiger_type::ERROR) {
                    return tiger_type::NIL;
                } else if (first_type != tiger_type::INT || last_type != tiger_type::INT) {
                    error_reporting();
                    cerr << "       'from' and 'to' expressions in 'for' loop must be integer expressions" << endl;
                    return tiger_type::ERROR;
                } else {
                    return tiger_type::ERROR;
                }
            }
    };

using ForLoopASTNode = QuaternaryASTNode<ForTo>;

template <typename Z>
    class UntypedVarDeclaration {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                string var_name = left_->toStr();
                tiger_type expr_type = right_->type_verify(scope);

                if (scope->preexisting(var_name)) {
                    error_reporting();
                    cerr << "       cannot redeclare variable " << var_name << " in the same scope" << endl;
                    return tiger_type::ERROR;
                }

                scope->symbol_insert(var_name, expr_type);
                return tiger_type::NIL;
            }
    };

using UntypedVarDeclASTNode = NoEvalBinaryASTNode<UntypedVarDeclaration>;

template <typename Z>
    class TypedVarDeclaration {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                std::string var_name = left_->toStr();
                std::string type_name = middle_->toStr();

                if (scope->preexisting(var_name)) {
                    error_reporting();
                    cerr << "       cannot redeclare variable " << var_name << " in the same scope" << endl;
                    return tiger_type::ERROR;
                }

                tiger_type value_type = right_->type_verify(scope);

                tiger_type var_type;
                if (type_name == "int") {
                    var_type = tiger_type::INT;
                } else if (type_name == "string") {
                    var_type = tiger_type::STRING;
                } else {
                    error_reporting();
                    cerr << "       unknown type " << type_name << endl;
                    return tiger_type::ERROR;
                }

                if (value_type == var_type) {
                    return tiger_type::NIL;
                } else {
                    error_reporting();
                    cerr << "       cannot assign expression of type <1%#!$?!?!> to variable "
                        << var_name << ", which is of type <!@#@#!@#>." << endl; // TODO replace this later
                    return tiger_type::ERROR;
                }
            }
    };

using TypedVarDeclASTNode = TertiaryASTNode<TypedVarDeclaration>;

template <typename Z>
    class TypeDeclaration {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {

                std::cout << "type decleration not implemented yet!!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using TypeDeclASTNode = NoEvalBinaryASTNode<TypeDeclaration>;

// e.g. let decl1 decl2 decl3 in exp1; exp2; exp3; end
template <typename Z>
    class LetBlock {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                // Create a new scope for the duration of the declaration section and body
                // section.
                scope-> push_scope();
                tiger_type declaration_type = left_->type_verify(scope);
                tiger_type body_type = right_->type_verify(scope);
                scope-> pop_scope();

                if (declaration_type != tiger_type::ERROR && body_type != tiger_type::ERROR) {
                    return body_type;
                } else {
                    return tiger_type::ERROR;
                }
            }
    };

using LetASTNode = NoEvalBinaryASTNode<LetBlock>;

// any type of declaration -- var, func, or type
template <typename Z>
    class Declaration {
        public:
            Z operator() (ASTNode::ASTptr child_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr child_) {
                return child_->type_verify(scope);
            }
    };

using DeclarationASTNode = NoEvalUnaryASTNode<Declaration>;

// a sequence of declarations (the first part of a let)
template <typename Z>
    class DeclList {
        public:
            Z operator() (std::vector<const DeclarationASTNode*> declarations) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, std::vector<const DeclarationASTNode*> vec_) {
                for (auto decl : vec_) {
                    tiger_type t = decl->type_verify(scope);
                    if (t == tiger_type::ERROR) {
                        return tiger_type::ERROR;
                    }
                }
                return tiger_type::NIL;
            }
    };

using DeclListASTNode = VectorASTNode<DeclList, DeclarationASTNode>;

// e.g. a; b; c; d;
template <typename Z>
    class ExprSeq {
        public:
            Z operator() (std::vector<const ASTNode*> members) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, std::vector<const ASTNode*> vec_) {
                tiger_type return_type = tiger_type::NIL;
                for (auto node : vec_) {
                    return_type = node->type_verify(scope);
                    if (return_type == tiger_type::ERROR) {
                        return tiger_type::ERROR;
                    }
                }
                return return_type;
            }
    };

using ExprSeqASTNode = VectorASTNode<ExprSeq, ASTNode>;

// e.g. a = b
template <typename Z>
    class FieldMember {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                std::cout << "field member not implemented yet!!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using FieldMemberASTNode = NoEvalBinaryASTNode<FieldMember>;

// e.g. a = b, c = d (in a type instantiation inside curly brackets)
template <typename Z>
    class FieldList {
        public:
            Z operator() (std::vector<const FieldMemberASTNode*> members) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, std::vector<const FieldMemberASTNode*> vec_) {
                std::cout << "fieldlsit not implemented yet!*" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using FieldListASTNode = VectorASTNode<FieldList, FieldMemberASTNode>;

// e.g. typename { field1 = val1, field2 = val2 }
template <typename Z>
    class TypeInstantiation {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                std::cout << "type instantiation not implemented yet!!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using TypeInstASTNode = NoEvalBinaryASTNode<TypeInstantiation>;

// the thing that goes on the right side of a type declaration
template <typename Z>
    class TypeValue {
        public:
            Z operator() (ASTNode::ASTptr child_) {
                return -1;
            }
            tiger_type type_verify(Scope* scope, ASTNode::ASTptr child_) {
                std::cout << "typevalue not implemented yet!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using TypeASTNode = NoEvalUnaryASTNode<TypeValue>;

// one field in a record type, e.g. a : int
template <typename Z>
    class RecordField {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                std::cout << "record not implemented yet!!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using RecordFieldASTNode = NoEvalBinaryASTNode<RecordField>;

// a record type -- i.e. a sequence of RecordFields
template <typename Z>
    class RecordType {
        public:
            Z operator() (std::vector<const RecordFieldASTNode*> fields) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, std::vector<const RecordFieldASTNode*> vec_) {
              set<string> string_set;
              cout << "checking vectors" << endl;
              for (unsigned int i = 0; i < vec_.size(); i++){
                string s = vec_[i]->toStr();
                string t = s.substr(0,s.find(":"));
                t.erase(remove(t.begin(), t.end(), ' '), t.end());
                string_set.insert(t);
              }
              if (string_set.size() != vec_.size()){
                error_reporting();
                cerr << "       repeated use of variable names" << endl;
                return tiger_type::ERROR;
              }

                std::cout << "record type not implemented yet!*" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using RecordTypeASTNode = VectorASTNode<RecordType, RecordFieldASTNode>;

// "array of" whatever type
template <typename Z>
    class ArrayTypeImplementation {
        public:
            Z operator() (ASTNode::ASTptr child_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr child_) {
                std::cout << "array not implemented yet!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using ArrayTypeASTNode = NoEvalUnaryASTNode<ArrayTypeImplementation>;

// represents lvalue.identifier
template <typename Z>
    class DotAccess {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                std::cout << "dot not implemented yet!!" << std::endl;
                return tiger_type::NOTIMPLEMENTED;
            }
    };

using DotASTNode = NoEvalBinaryASTNode<DotAccess>;

// represents array[index]
template <typename Z>
    class IndexAccess {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                // TODO check left side is an array
                //tiger_type left_type = left_->type_verify(scope);
                tiger_type index_type = right_->type_verify(scope);
                if (index_type != tiger_type::INT) {
                    error_reporting();
                    cerr << "       index of the array must be an integer expression" << endl;
                    return tiger_type::ERROR;
                } else {
                    // TODO return type the array is of
                    return tiger_type::INT;
                }
            }
    };

using IndexASTNode = NoEvalBinaryASTNode<IndexAccess>;

// represents array[index]
template <typename Z>
    class ArrayValue {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                // TODO check left side is an array
                //tiger_type left_type = left_->type_verify(scope);
                tiger_type index_type = right_->type_verify(scope);
                if (index_type != tiger_type::INT) {
                    error_reporting();
                    cerr << "       index of the array must be an integer expression" << endl;
                    return tiger_type::ERROR;
                } else {
                    // TODO return type the array is of
                    return tiger_type::INT;
                }
            }
    };

using ArrayASTNode = TertiaryASTNode<ArrayValue>;

// represents function declerations
template <typename Z>
    class UnTypedFuncDecl {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                // TODO make this actually declare something
                string func_name = left_->toStr();
                if (scope->preexisting(func_name)) {
                    error_reporting();
                    cerr << "       cannot redeclare function " << func_name << " in the same scope" << endl;
                    return tiger_type::ERROR;
                }

                tiger_type arguments = middle_->type_verify(scope);
                if (arguments == tiger_type::ERROR){
                  return tiger_type::ERROR;
                }
                // TODO make this take a scope that has the parameters in it
                tiger_type return_type = right_->type_verify(scope);

                scope->symbol_insert(func_name, return_type);

                return tiger_type::NIL;
            }
    };

using UnTypedFuncDeclASTNode = TertiaryASTNode<UnTypedFuncDecl>;

// represents function declerations
template <typename Z>
    class TypedFuncDecl {
        public:
            Z operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
                // TODO make this actually declare something
                string func_name = one_->toStr();

                string declared_func_type = two_->toStr();
                // TODO make this take a scope that has the parameters in it
                tiger_type return_type = four_->type_verify(scope);

                if (scope->preexisting(func_name)) {
                    error_reporting();
                    cerr << "       cannot redeclare function " << func_name << " in the same scope" << endl;
                    return tiger_type::ERROR;
                }

                // TODO compare types here
                if (declared_func_type == "int" && return_type == tiger_type::INT) {
                    // it's an int
                } else if (declared_func_type == "string" && return_type == tiger_type::STRING) {
                    // it's a string
                } else if (return_type != tiger_type::ERROR) {
                    cerr << "       function " << func_name << " declared as returning <%$#$#@>, "
                         << "but evaluates to <#$@$#>" << endl;
                    return tiger_type::ERROR;
                } else {
                    return tiger_type::ERROR;
                }

                scope->symbol_insert(func_name, return_type);

                return tiger_type::NIL;
            }
    };

using TypedFuncDeclASTNode = QuaternaryASTNode<TypedFuncDecl>;

// represents function calls
template <typename Z>
    class FuncCall {
        public:
            Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            tiger_type type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                // TODO when we have function types, look up the function by return type and
                // verify argument types correct
                string func_name = left_->toStr();

                tiger_type return_type = scope->search(func_name);

                if (return_type == tiger_type::ERROR) {
                    error_reporting();
                    cerr << "       unknown function " << func_name << endl;
                    return tiger_type::ERROR;
                }

                return return_type;
            }
    };

using FuncCallASTNode = NoEvalBinaryASTNode<FuncCall>;

} // namespace

#endif
