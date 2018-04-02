#ifndef _AST_HH_
#define _AST_HH_


#include <iostream>
#include <cstdio>
#include <sstream>
#include <functional>
#include <string>
#include <cmath>
#include <set>
#include "type.hh"
#include "scope.hh"

using namespace std;

namespace tiger {

    ///////////////////////////////////////////////////////////////////////////////
    // Base AST node class, to define the hierarchy.
    class ASTNode {
        public:
            using value_t = int;  // All values will be floating-point
            using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

            ASTNode() = default;
            virtual ~ASTNode() = default;
            virtual const Type *type_verify(Scope* scope) const = 0; // Determine type of expression
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

            virtual const Type *type_verify(Scope* scope) const {
                return Type::nilType;
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
            BreakASTNode(int loc)
                : ASTNode(), location_(loc)
            {}
            virtual ~BreakASTNode() = default;

            virtual const Type *type_verify(Scope* scope) const {
                return Type::nilType;
            }

            virtual value_t eval() const
            {
                return -1; /// FIX ME
            }

            virtual std::string toStr() const
            {
                return "break";
            }

        private:
            int location_;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // A node type that evaluates to a numeric constant:
    class NumASTNode : public ASTNode {
        public:
            NumASTNode(value_t value)
                : ASTNode(), value_(value)
            {}
            virtual ~NumASTNode() = default;

            virtual const Type *type_verify(Scope* scope) const {
                return Type::intType;
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

            virtual const Type *type_verify(Scope* scope) const {
                return Type::stringType;
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
            NameASTNode(std::string value, int location)
                : ASTNode(), value_(value), location_(location)
            {}
            virtual ~NameASTNode() = default;

            value_t eval() const {
                return -1; /// FIX ME
            }

            virtual const Type *type_verify(Scope* scope) const {
                const Type *var_type = scope->search(value_);
                if (var_type == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       unknown variable ‘" << value_ << "’" << endl;
                    return Type::errorType;
                }
                return var_type;
            }

            virtual std::string toStr() const {
                return value_;
            }


        private:
            const std::string value_;
            int location_;
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
                UnaryASTNode(std::string rep, ASTptr child, int loc)
                    : ASTNode(), rep_(rep), child_(child), location_(loc)
                {}

                virtual ~UnaryASTNode()
                {
                    delete child_;
                }

                virtual const Type *type_verify(Scope* scope) const {
                    const Type *child_type = child_->type_verify(scope);
                    if (child_type == Type::intType) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       unary operation ‘" << rep_ << "’ applied to non-integer value ‘"
                            << child_->toStr() << "’" << endl;
                        return Type::errorType;
                    }
                    return Type::intType;
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
                int location_;
        };

    // Example usage of UnaryASTNode:
    using UnaryMinusASTNode = UnaryASTNode<std::negate>;

    //////////////////////////////////////////////////////////////////////////
    // A unary AST node that doesn't evaluate its argument when calling eval.
    template <class O>
        class NoEvalUnaryASTNode : public ASTNode {
            public:
                // Take a single child to evalute, and a string representation of the node:
                NoEvalUnaryASTNode(std::string rep, ASTptr child, int loc)
                    : ASTNode(), rep1_(rep), rep2_(""), child_(child), location_(loc)
                {}

                NoEvalUnaryASTNode(std::string rep1, std::string rep2, ASTptr child, int loc)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), child_(child), location_(loc)
                {}

                virtual ~NoEvalUnaryASTNode()
                {
                    delete child_;
                }

                virtual const Type *type_verify(Scope* scope) const {
                    auto op = O();
                    return op.type_verify(scope, child_, location_);
                }

                value_t eval() const
                {
                    auto op = O();
                    return op(child_);
                }

                virtual std::string toStr() const
                {
                    return rep1_ + child_->toStr() + rep2_;
                }

            private:
                const std::string rep1_, rep2_;  // String representation of node
                const ASTptr child_;
                int location_;
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
                BinaryASTNode(std::string rep, ASTptr left, ASTptr right, int loc)
                    : ASTNode(), rep1_(""), rep2_(rep), left_(left), right_(right), location_(loc)
                {}

                BinaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr right, int loc)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), left_(left), right_(right), location_(loc)
                {}

                virtual ~BinaryASTNode() {
                    delete left_;
                    delete right_;
                }

                virtual const Type *type_verify(Scope* scope) const {
                    const Type *left_type = left_->type_verify(scope);
                    const Type *right_type = right_->type_verify(scope);
                    if (left_type == Type::intType && right_type == Type::intType) {
                        return Type::intType;
                    } else if (left_type == Type::errorType || right_type == Type::errorType) {
                        /* this means the error was further down below, so we just pass
                         * it up again */
                        return Type::errorType;
                    } else {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       in expression " << toStr() << endl;
                        cerr << "       Attempting binary operation on between 1 or more non-integer values" << endl;
                        cerr << "       (‘" << left_->toStr() << "’ is of type ‘" << left_type->toStr() << "’ and ‘"
                            << right_->toStr() << "’ is of type ‘"<< right_type->toStr() << "’)" << endl;
                        return Type::errorType;
                    }
                }

                value_t eval() const {
                    auto op = O<value_t>();
                    return op(left_->eval(), right_->eval());
                }

                virtual std::string toStr() const {
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
                int location_;
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
    template <class O>
        class NoEvalBinaryASTNode : public ASTNode {
            public:
                // parens flag = whether to surround with parens and insert spaces around operators
                NoEvalBinaryASTNode(std::string rep, ASTptr left, ASTptr right, int loc, bool parens=true)
                    : ASTNode(), rep1_(""), rep2_(rep), left_(left), right_(right), parens_(parens), location_(loc)
                {}

                NoEvalBinaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr right, int loc, bool parens=true)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), left_(left), right_(right), parens_(parens), location_(loc)
                {}

                NoEvalBinaryASTNode(std::string rep1, std::string rep2, std::string rep3, ASTptr left, ASTptr right, int loc, bool parens=true)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), left_(left), right_(right), parens_(parens), location_(loc)
                {}

                virtual ~NoEvalBinaryASTNode()
                {
                    delete left_;
                    delete right_;
                }

                value_t eval() const
                {
                    auto op = O();
                    return op(left_, right_);
                }

                virtual const Type *type_verify(Scope* scope) const {
                    auto op = O();
                    return op.type_verify(scope, left_, right_, location_);
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
                int location_;
        };

    //////////////////////////////////////////////////////////////////////////////
    // A node type for tertiary operators.
    // (i.e. if-then-else)
    template <class O>
        class TertiaryASTNode : public ASTNode {
            public:
                TertiaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4,
                        ASTptr left, ASTptr middle, ASTptr right, int loc, bool parens=true)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), left_(left),
                    middle_(middle), right_(right), parens_(parens), location_(loc)
            {}

                TertiaryASTNode(std::string rep1, std::string rep2, std::string rep3,
                        ASTptr left, ASTptr middle, ASTptr right, int loc, bool parens=true)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(""), left_(left),
                    middle_(middle), right_(right), parens_(parens), location_(loc)
            {}

                TertiaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr middle, ASTptr right,
                        int loc, bool parens=true)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(""), rep4_(""), left_(left), middle_(middle),
                    right_(right), parens_(parens), location_(loc)
            {}

                virtual ~TertiaryASTNode()
                {
                    delete left_;
                    delete middle_;
                    delete right_;
                }

                virtual const Type *type_verify(Scope* scope) const {
                    auto op = O();
                    return op.type_verify(scope, left_, middle_, right_, location_);
                }

                value_t eval() const
                {
                    auto op = O();
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
                int location_;
        };


    //////////////////////////////////////////////////////////////////////////////
    // A node type for tertiary operators.
    // (i.e. if-then-else)
    template <class O>
        class QuaternaryASTNode : public ASTNode {
            public:
                QuaternaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4,
                        ASTptr one, ASTptr two, ASTptr three, ASTptr four, int loc, bool parens=true)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), rep5_(""),
                    one_(one), two_(two), three_(three), four_(four), parens_(parens), location_(loc)
            {}

                QuaternaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4, std::string rep5,
                        ASTptr one, ASTptr two, ASTptr three, ASTptr four, int loc, bool parens=true)
                    : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), rep5_(rep5),
                    one_(one), two_(two), three_(three), four_(four), parens_(parens), location_(loc)
            {}

                virtual ~QuaternaryASTNode()
                {
                    delete one_;
                    delete two_;
                    delete three_;
                    delete four_;
                }

                virtual const Type *type_verify(Scope* scope) const {
                    auto op = O();
                    return op.type_verify(scope, one_, two_, three_, four_, location_);
                }

                value_t eval() const
                {
                    auto op = O();
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
                int location_;
        };


    //////////////////////////////////////////////////////////////////////////////
    // A node type for vectors of AST nodes.
    // (e.g. declaration lists, parameter lists, statement sequences, ....)
    template <class O, class E> // E is the elements of the vector (?)
        class VectorASTNode : public ASTNode {
            public:
                using VASTptr = const VectorASTNode*; // Can't use smart ptr in union :(

                VectorASTNode(std::string separator, int loc)
                    : ASTNode(), sep_(separator), first_(""), last_(""), location_(loc)
                {}

                VectorASTNode(std::string separator, std::string first, std::string last, int loc)
                    : ASTNode(), sep_(separator), first_(first), last_(last), location_(loc)
                {}

                virtual ~VectorASTNode() {
                    for (auto a : vec_) {
                        delete a;
                    }
                }

                virtual const Type *type_verify(Scope* scope) const {
                    auto op = O();
                    return op.type_verify(scope, vec_, location_);
                }

                value_t eval() const
                {
                    auto op = O();
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
                    for (unsigned i = 0; i < vec_.size(); i++) {
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
                int location_;
        };


    class Assignment {
        public:
            // TODO implement variable assignment
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                const Type *name_type = left_->type_verify(scope);
                const Type *value_type = right_->type_verify(scope);
                if (name_type == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       unknown variable ‘" << left_->toStr() << "’" << endl;
                    return Type::errorType;
                }

                if (name_type == value_type && name_type != Type::errorType) {
                    return value_type;
                } else if (name_type != Type::errorType && value_type != Type::errorType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot assign expression of type ‘" << value_type->toStr() << "’ to variable ‘"
                        << left_->toStr() << "’, which is of type ‘" << name_type->toStr() << "’." << endl;
                    return Type::errorType;
                } else {
                    return Type::errorType;
                }
            }
    };

    using AssignASTNode = NoEvalBinaryASTNode<Assignment>;


    class IfThenElse {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                if (left_->eval()) {
                    return middle_->eval();
                } else if (right_ != NULL) {
                    return right_->eval();
                } else {
                    return -1; /// FIX ME
                }
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_);
    };

    using ConditionalASTNode = TertiaryASTNode<IfThenElse>;


    class WhileDo {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                ASTNode::value_t result = -1; // WHAT SHOULD IT RETURN
                while (left_->eval()) {
                    result = right_->eval();
                }
                return result;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);
    };

    using WhileLoopASTNode = NoEvalBinaryASTNode<WhileDo>;


    class ForTo {
        public:
            // TODO this is not actually right, we don't have binding yet?
            ASTNode::value_t operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr one_, ASTNode::ASTptr two_,
                                    ASTNode::ASTptr three_, ASTNode::ASTptr four_, int location_);
    };

    using ForLoopASTNode = QuaternaryASTNode<ForTo>;


    class UntypedVarDeclaration {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                string var_name = left_->toStr();
                const Type *expr_type = right_->type_verify(scope);

                if (scope->preexisting(var_name)) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot redeclare variable ‘" << var_name << "’ in the same scope" << endl;

                    return Type::errorType;
                }

                scope->symbol_insert(var_name, expr_type);
                return Type::nilType;
            }
    };

    using UntypedVarDeclASTNode = NoEvalBinaryASTNode<UntypedVarDeclaration>;


    class TypedVarDeclaration {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_) {
                std::string var_name = left_->toStr();
                std::string type_name = middle_->toStr();

                if (scope->preexisting(var_name)) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot redeclare variable ‘" << var_name << "’ in the same scope" << endl;
                    return Type::errorType;
                }

                const Type *value_type = right_->type_verify(scope);

                const Type *var_type = scope->type_search(type_name);
                if (var_type == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       variable ‘" << var_name << "’ declared as unknown type ‘" << type_name << "’" << endl;
                    return Type::errorType;
                }

                if (value_type->equivalent(var_type)) {
                    scope->symbol_insert(var_name, var_type);

                    return Type::nilType;
                } else {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       variable ‘" << left_->toStr() << "’ declared as type ‘" <<
                        var_type->toStr() << "’ but is being assigned type ‘" << value_type->toStr() << "’" << endl;
                    return Type::errorType;
                }
            }
    };

    using TypedVarDeclASTNode = TertiaryASTNode<TypedVarDeclaration>;


    class TypeDeclaration {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                string type_name = left_->toStr();
                const Type *new_type = right_->type_verify(scope);

                if (new_type->getKind() == tiger_type::RECORD) {
                    /* the record type wants to know the name it's assigned to, but these
                     * types are const, so we create a new one and delete the old one. */
                    RecordType *new_rec = new RecordType(type_name, static_cast<const RecordType*>(new_type));
                    delete static_cast<const RecordType*>(new_type);
                    new_type = new_rec;
                }

                scope->type_insert(type_name, new_type);

                if (new_type == Type::errorType) {
                    return Type::errorType;
                } else {
                    return Type::nilType;
                }
            }
    };

    using TypeDeclASTNode = NoEvalBinaryASTNode<TypeDeclaration>;

    // e.g. let decl1 decl2 decl3 in exp1; exp2; exp3; end

    class LetBlock {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                // Create a new scope for the duration of the declaration section and body
                // section.
                scope->push_scope();
                const Type *declaration_type = left_->type_verify(scope);
                const Type *body_type;
                if (declaration_type != Type::errorType) {
                    body_type = right_->type_verify(scope);
                } else {
                    body_type = Type::errorType;
                }
                scope->pop_scope();

                if (declaration_type != Type::errorType && body_type != Type::errorType) {
                    return body_type;
                } else {
                    return Type::errorType;
                }
            }
    };

    using LetASTNode = NoEvalBinaryASTNode<LetBlock>;

    // any type of declaration -- var, func, or type

    class Declaration {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr child_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr child_, int location_) {
                return child_->type_verify(scope);
            }
    };

    using DeclarationASTNode = NoEvalUnaryASTNode<Declaration>;

    // a sequence of declarations (the first part of a let)

    class DeclList {
        public:
            ASTNode::value_t operator() (std::vector<const DeclarationASTNode*> declarations) {
                return -1;
            }

            const Type *type_verify(Scope* scope, std::vector<const DeclarationASTNode*> vec_, int location_) {
                for (auto decl : vec_) {
                    const Type *t = decl->type_verify(scope);
                    if (t == Type::errorType) {
                        return Type::errorType;
                    }
                }
                return Type::nilType;
            }
    };

    using DeclListASTNode = VectorASTNode<DeclList, DeclarationASTNode>;

    // e.g. a; b; c; d; OR a, b, c, d

    class ExprSeq {
        public:
            ASTNode::value_t operator() (std::vector<const ASTNode*> members) {
                return -1;
            }

            const Type *type_verify(Scope* scope, std::vector<const ASTNode*> vec_, int location_) {
                /* Find out whether we're checking if these match function parameters,
                 * or if we're just evaluating them. */
                /* (We used the same AST node type for both.) */
                const Type *check_type = scope->search("_checked_params");

                if (check_type == Type::notFoundType) {
                    /* It's just a sequence of expressions to evaluate
                     * (semicolons) */
                    const Type *return_type = Type::nilType;
                    for (auto node : vec_) {
                        return_type = node->type_verify(scope);
                        if (return_type == Type::errorType) {
                            return Type::errorType;
                        }
                    }
                    return return_type;
                } else {
                    /* It's a set of function parameters (commas) */
                    if (check_type->getKind() != tiger_type::RECORD) {
                        // this should only be passed in by the function call checker
                        cerr << "how did you get here!" << endl;
                        return Type::errorType;
                    }

                    const RecordType *params = static_cast<const RecordType*>(check_type);

                    const Type *func_name = scope->search("_checked_funcname");

                    if (func_name->getKind() != tiger_type::RECORD) {
                        // again we should only end up here from the function call checker
                        cerr << "how did you get here!!" << endl;
                        return Type::errorType;
                    }

                    if (vec_.size() > params->fields_.size()) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       too many parameters to function ‘" << func_name->toStr()
                            << "’ (expected " << params->fields_.size() << ", got " << vec_.size() << ")" << endl;
                        return Type::errorType;
                    } else if (vec_.size() < params->fields_.size()) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       not enough parameters to function ‘" << func_name->toStr()
                            << "’ (expected " << params->fields_.size() << ", got " << vec_.size() << ")" << endl;
                        return Type::errorType;
                    }

                    /* Now check that the types of the parameters match. */
                    scope->push_scope();

                    /* Hide _checked_params when we check types for the parameters */
                    //scope->symbol_insert("_checked_params", Type::notFoundType);

                    for (unsigned i = 0; i < vec_.size(); i++) {
                        const Type *arg_type = vec_[i]->type_verify(scope);

                        if (arg_type == Type::errorType) {
                            return Type::errorType;
                        }

                        const Type *expected_type = params->fields_[i].second;

                        if (arg_type != expected_type) {
                            cerr << "ERROR: line " << location_ << endl;
                            cerr << "       parameter ‘" << params->fields_[i].first << "’ to function ‘"
                                << func_name->toStr() << "’ declared as type ‘" << expected_type->toStr()
                                << "’ but received expression ‘" << vec_[i]->toStr() << "’, which is of type ‘"
                                << arg_type->toStr() << "’." << endl;
                            return Type::errorType;
                        }
                    }

                    scope->pop_scope();

                    return Type::nilType;
                }
            }
    };

    using ExprSeqASTNode = VectorASTNode<ExprSeq, ASTNode>;

    // e.g. a = b

    class FieldMember {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                return right_->type_verify(scope);
            }
    };

    using FieldMemberASTNode = NoEvalBinaryASTNode<FieldMember>;

    // e.g. a = b, c = d (in a type instantiation inside curly brackets)

    class FieldList {
        public:
            ASTNode::value_t operator() (std::vector<const FieldMemberASTNode*> members) {
                return -1;
            }

            const Type *type_verify(Scope* scope, std::vector<const FieldMemberASTNode*> vec_, int location_) {
                /* Similar to the declaration, we parse the toStr's to get
                 * the field names and make sure there are no duplicates. */
                set<string> string_set;

                /* Get the currently-instantiating record type from the TypeInstantiation node
                 * above us. */
                const Type *insttype = scope->type_search("_current_rectype");

                if (insttype->getKind() != tiger_type::RECORD) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot instantiate non-record type ‘" << insttype->toStr() << "’" << endl;
                    return Type::errorType;
                }

                if (insttype == Type::notFoundType) {
                    /* we should only be able to get here from a TypeInstantiation calling us
                     * with this thing set in the scope */
                    cerr << "how did you get here???" << endl;
                    return Type::errorType;
                }

                const RecordType *rectype = static_cast<const RecordType*>(insttype);

                /* Keep track of which fields have yet to be declared, so we can report when
                 * they are absent. */
                set<string> undeclared;
                for (unsigned int i = 0; i < rectype->fields_.size(); i++) {
                    undeclared.insert(rectype->fields_[i].first);
                }

                for (unsigned int i = 0; i < vec_.size(); i++) {
                    string s = vec_[i]->toStr();
                    string t = s.substr(0,s.find("="));
                    t.erase(remove(t.begin(), t.end(), ' '), t.end());
                    t.erase(remove(t.begin(), t.end(), '('), t.end());

                    const Type *field_type = rectype->field_type(t);

                    if (field_type == Type::notFoundType) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       type ‘" << rectype->toStr() << "’ has no field ‘" << t << "’" << endl;
                        return Type::errorType;
                    }

                    if (string_set.count(t) > 0) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       field ‘" << t << "’ initialized multiple times in instantiation of record type ‘"
                            << rectype->toStr() << "’" << endl;
                        return Type::errorType;
                    }

                    const Type *value_type = vec_[i]->type_verify(scope);
                    if (value_type != field_type) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       in expression ‘" << vec_[i]->toStr() << "’" << endl;
                        cerr << "       field ‘" << t << "’ of record type ‘" << rectype->toStr() << "’ declared as type ‘"
                            << field_type->toStr() << "’, but is being assigned a value of type ‘"
                            << value_type->toStr() << "’" << endl;
                        return Type::errorType;
                    }

                    undeclared.erase(t);

                    string_set.insert(t);
                }

                if (undeclared.size() > 0) {
                    cerr << "ERROR: line " << location_ << endl;
                    if (undeclared.size() == 1) {
                        cerr << "       no value given for field ‘" << *(undeclared.begin())
                            << "’ of record type ‘" << rectype->toStr() << "’" << endl;
                    } else if (undeclared.size() == 2) {
                        cerr << "       no value given for field ‘" << *(undeclared.begin())
                            << "’ or ‘" << *(next(undeclared.begin()))
                            << "’ of record type ‘" << rectype->toStr() << "’" << endl;
                    } else {
                        cerr << "       no value given for field ";
                        for (auto it = undeclared.begin(); it != undeclared.end(); it++) {
                            cerr << "‘" << *it << "’";
                            if (next(it) == undeclared.end()) {
                                /* at end, don't do anything */
                            } else if (next(next(it)) == undeclared.end()) {
                                cerr << ", or ";
                            } else {
                                cerr << ", ";
                            }
                        }
                        cerr << " of record type ‘" << rectype->toStr() << "’" << endl;
                    }
                    return Type::errorType;
                }

                return rectype;
            }
    };

    using FieldListASTNode = VectorASTNode<FieldList, FieldMemberASTNode>;

    // e.g. typename { field1 = val1, field2 = val2 }

    class TypeInstantiation {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                const Type *instantiating_type = scope->type_search(left_->toStr());

                if (instantiating_type == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot instantiate nonexistent record type ‘" << left_->toStr() << "’" << endl;
                    return Type::errorType;
                }

                scope->push_scope();

                /* We do this to pass the attempted instantiating type to the child node,
                 * so that it can check whether the fields are valid. */
                scope->type_insert("_current_rectype", instantiating_type);

                const Type *instantiated_type = right_->type_verify(scope);

                scope->pop_scope();

                if (instantiated_type == Type::errorType) {
                    return Type::errorType;
                }

                return instantiated_type;
            }
    };

    using TypeInstASTNode = NoEvalBinaryASTNode<TypeInstantiation>;

    // the thing that goes on the right side of a type declaration

    class TypeValue {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr child_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr child_, int location_) {
                return child_->type_verify(scope);
            }
    };

    using TypeASTNode = NoEvalUnaryASTNode<TypeValue>;

    // one field in a record type, e.g. a : int

    class RecordField {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                return scope->type_search(right_->toStr());
            }
    };

    using RecordFieldASTNode = NoEvalBinaryASTNode<RecordField>;

    // a record type -- i.e. a sequence of RecordFields

    class RecordTypeAST {
        public:
            ASTNode::value_t operator() (std::vector<const RecordFieldASTNode*> fields) {
                return -1;
            }

            const Type *type_verify(Scope* scope, std::vector<const RecordFieldASTNode*> vec_, int location_) {
                /* dealing with multiple declerations (function arguments)
                   using sets allows us to check the count on a given variable
                   name, if there are more duplicates, return errorType */
                set<string> string_set;

                RecordType *rec = new RecordType();

                for (unsigned i = 0; i < vec_.size(); i++){
                    string s = vec_[i]->toStr();
                    string t = s.substr(0,s.find(":"));
                    t.erase(remove(t.begin(), t.end(), ' '), t.end());

                    if (string_set.count(t) > 0) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       name ‘" << t << "’ used multiple times in function or record declaration" << endl;
                        delete rec;
                        return Type::errorType;
                    }

                    string_set.insert(t);

                    const Type *field_type = vec_[i]->type_verify(scope);

                    if (field_type == Type::notFoundType) {
                        cerr << "ERROR: line " << location_ << endl;
                        cerr << "       record field or function parameter ‘" << t << "’ declared as nonexistent type ‘"
                            << vec_[i]->toStr() << "’" << endl;
                        delete rec;
                        return Type::errorType;
                    }

                    rec->add_field(t, field_type);
                }

                return rec;
            }
    };

    using RecordTypeASTNode = VectorASTNode<RecordTypeAST, RecordFieldASTNode>;

    // "array of" whatever type

    class ArrayTypeAST {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr child_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr child_, int location_) {
                string type_name = child_->toStr();
                const Type *arrayof = scope->type_search(type_name);

                if (arrayof == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot create array of nonexistent type ‘" << type_name << "’" << endl;
                    return Type::errorType;
                }

                return new ArrayType(arrayof);
            }
    };

    using ArrayTypeASTNode = NoEvalUnaryASTNode<ArrayTypeAST>;

    // represents lvalue.identifier

    class DotAccess {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                const Type *accessed_type = left_->type_verify(scope);
                if (accessed_type->getKind() != tiger_type::RECORD) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       attempt to access field ‘" << right_->toStr() << "’ of expression ‘"
                        << left_->toStr() << "’, which is of non-record type ‘" << accessed_type->toStr() << "’" << endl;
                    return Type::errorType;
                }

                const RecordType *rectype = static_cast<const RecordType*>(accessed_type);

                if (rectype->field_type(right_->toStr()) == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       in expression ‘" << left_->toStr() << "." << right_->toStr() << "’" << endl;
                    cerr << "       ‘" << left_->toStr() << "’ is of type ‘" << rectype->toStr() <<
                        "’, which has no field ‘" << right_->toStr() << "’" << endl;
                    return Type::errorType;
                }

                return rectype->field_type(right_->toStr());
            }
    };

    using DotASTNode = NoEvalBinaryASTNode<DotAccess>;

    // represents array[index]

    class IndexAccess {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                const Type *left_type = left_->type_verify(scope);
                if (left_type->getKind() != tiger_type::ARRAY) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       attempt to index into ‘" << left_->toStr() << "’, which is not an array" << endl;
                    cerr << "       (it is of type ‘" << left_type->toStr() << "’.)" << endl;
                    return Type::errorType;
                }

                const Type *index_type = right_->type_verify(scope);
                if (index_type != Type::intType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       index of the array must be an integer expression" << endl;
                    cerr << "       (got ‘" << right_->toStr() << "’, which is of type ‘" << index_type->toStr() << "’)" << endl;

                    return Type::errorType;
                } else {
                    // We know it's an ArrayType, because if it wasn't, we would have thrown
                    // it away up there.
                    return static_cast<const ArrayType*>(left_type)->type_of_;
                }
            }
    };

    using IndexASTNode = NoEvalBinaryASTNode<IndexAccess>;

    // represents type[length] of value

    class ArrayValue {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_) {
                string type_name = left_->toStr();

                const Type *length_type = middle_->type_verify(scope);

                if (length_type != Type::intType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       length of the array must be an integer expression" << endl;
                    cerr << "       (got ‘" << middle_->toStr() << "’, which is of type ‘" << length_type->toStr() << "’." << endl;
                    return Type::errorType;
                }

                const Type *arr_type = scope->type_search(type_name);

                const Type *value_type = right_->type_verify(scope);

                if (!arr_type->equivalent(value_type)) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       declaring an array of values of type ‘" << type_name << "’, but default value given is ‘" <<
                        right_->toStr() << "’, which is of type ‘" << value_type->toStr() << "’" << endl;
                    return Type::errorType;
                }

                return new ArrayType(value_type);
            }
    };

    using ArrayASTNode = TertiaryASTNode<ArrayValue>;

    // represents function declerations

    class UnTypedFuncDecl {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_) {
                string func_name = left_->toStr();

                if (scope->preexisting(func_name)) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot redeclare function ‘" << func_name << "’ in the same scope" << endl;
                    return Type::errorType;
                }

                const Type *argtype = middle_->type_verify(scope);
                if (argtype == Type::errorType){
                    return Type::errorType;
                }

                if (argtype->getKind() != tiger_type::RECORD) {
                    /* this should have been a syntax error */
                    cerr << "how did you get here" << endl;
                    return Type::errorType;
                }

                const RecordType *arguments = static_cast<const RecordType*>(argtype);

                // add the parameters into the scope, so we can check them in function body

                scope->push_scope();

                /* We will just disallow calling functions recursively with no declared return type. */

                scope->symbol_insert(func_name, Type::incompleteRecursiveType);

                for (auto a : arguments->fields_) {
                    scope->symbol_insert(a.first, a.second);
                }

                const Type *return_type = right_->type_verify(scope);

                scope->pop_scope();

                FunctionType *functype = new FunctionType(func_name, arguments, return_type);

                scope->symbol_insert(func_name, functype);

                return Type::nilType;
            }
    };

    using UnTypedFuncDeclASTNode = TertiaryASTNode<UnTypedFuncDecl>;

    // represents function declerations

    class TypedFuncDecl {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr name_node, ASTNode::ASTptr params_node, ASTNode::ASTptr rettype_node, ASTNode::ASTptr funcbody_node, int location_) {
                // one: func name
                // two: func params
                // three: func return type
                // four: func body
                string func_name = name_node->toStr();

                const Type *declared_func_type = scope->type_search(rettype_node->toStr());

                if (declared_func_type == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       function ‘" << func_name << "’ declared as returning nonexistent type ‘" <<
                        rettype_node->toStr() << "’" << endl;
                    return Type::errorType;
                }

                if (scope->preexisting(func_name)) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot redeclare function ‘" << func_name << "’ in the same scope" << endl;

                    return Type::errorType;
                }

                const Type *argtype = params_node->type_verify(scope);
                if (argtype == Type::errorType){
                    return Type::errorType;
                }

                if (argtype->getKind() != tiger_type::RECORD) {
                    /* this should have been a syntax error */
                    cerr << "how did you get here" << endl;
                    return Type::errorType;
                }

                const RecordType *arguments = static_cast<const RecordType*>(argtype);

                // add the parameters into the scope, so we can check them in function body
                // also add the function type itself before we check the body, so that we can
                // account for recursive function calls

                FunctionType *functype = new FunctionType(func_name, arguments, declared_func_type);

                scope->symbol_insert(func_name, functype);

                scope->push_scope();

                for (auto a : arguments->fields_) {
                    scope->symbol_insert(a.first, a.second);
                }

                const Type *return_type = funcbody_node->type_verify(scope);

                if (return_type == declared_func_type) {
                    // we're good!
                } else if (return_type != Type::errorType && declared_func_type != Type::errorType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       function ‘" << func_name << "’ declared as returning ‘" << declared_func_type->toStr()
                        << "’, but evaluates to ‘" << return_type->toStr() << "’" << endl;
                    return Type::errorType;
                } else {
                    return Type::errorType;
                }

                scope->pop_scope();

                return Type::nilType;
            }
    };

    using TypedFuncDeclASTNode = QuaternaryASTNode<TypedFuncDecl>;

    // represents function calls

    class FuncCall {
        public:
            ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
                return -1;
            }

            const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
                string func_name = left_->toStr();

                const Type *var_type = scope->search(func_name);

                if (var_type == Type::notFoundType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       unknown function ‘" << func_name << "’" << endl;
                    return Type::errorType;
                } else if (var_type == Type::incompleteRecursiveType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       cannot recursively call function ‘" << func_name
                        << "’, which has no declared return type" << endl;
                    return Type::errorType;
                } else if (var_type->getKind() != tiger_type::FUNCTION) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       in expression ‘" << left_->toStr() << "(" << right_->toStr() << ")’" << endl;
                    cerr << "       variable ‘" << left_->toStr() << "’ is not a function" << endl;
                    return Type::errorType;
                }

                const FunctionType *func_type = static_cast<const FunctionType*>(var_type);

                const RecordType *arg_type = func_type->args_;

                const RecordType *dummy_func_name = new RecordType(func_name);

                scope->push_scope();

                /* Pass the checked parameters to the expression-sequence node. */
                scope->symbol_insert("_checked_params", arg_type);
                /* Pass the name of the current function to the expr-seq node, for better
                 * error messages. */
                scope->symbol_insert("_checked_funcname", dummy_func_name);

                const Type *param_check_result = right_->type_verify(scope);

                if (param_check_result == Type::errorType) {
                    delete dummy_func_name;
                    scope->pop_scope();
                    return Type::errorType;
                }

                scope->pop_scope();

                delete dummy_func_name;

                const Type *return_type = func_type->rettype_;

                return return_type;
            }
    };

    using FuncCallASTNode = NoEvalBinaryASTNode<FuncCall>;

} // namespace

#endif
