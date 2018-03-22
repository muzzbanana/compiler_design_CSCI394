#ifndef _AST_HH_
#define _AST_HH_
/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/RCS/fb3-1.h,v 2.1 2009/11/08 02:53:18 johnl Exp $
 */
/*
 * Declarations for a calculator fb3-1
 */

#include <iostream>
#include <sstream>
#include <functional>
#include <string>
#include <cmath>

/* interface to the lexer */
//void yyerror(tiger::ASTNode::ASTptr *out, char *s, ...);

namespace tiger {

///////////////////////////////////////////////////////////////////////////////
// Base AST node class, to define the hierarchy.
class ASTNode {
 public:
  using value_t = double;  // All values will be floating-point
  using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

  ASTNode() = default;
  virtual ~ASTNode() = default;
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

  virtual std::string toStr() const
  {
    return value_;
  }


 private:
  const std::string value_;
};


///////////////////////////////////////////////////////////////////////////////
// Example node: absolute value. This could be implemented in terms of
// UnaryASTNode by defining a functor class for absolute value.
class AbsASTNode : public ASTNode {
 public:
  AbsASTNode(ASTptr child)
   : ASTNode(), child_(child)
  {}

  virtual ~AbsASTNode()
  {
    delete child_;
  }

  value_t eval() const
  {
    return std::abs(child_->eval());
  }

  virtual std::string toStr() const
  {
    return "|" + child_->toStr() + "|";
  }

 private:
  const ASTptr child_;   // Sub-expression
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
};

using WhileLoopASTNode = NoEvalBinaryASTNode<WhileDo>;

template <typename Z>
class ForTo {
    public:
        // TODO this is not actually right, we don't have binding yet?
        Z operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
            return -1;
        }
};

using ForLoopASTNode = QuaternaryASTNode<ForTo>;

template <typename Z>
class UntypedVarDeclaration {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using UntypedVarDeclASTNode = NoEvalBinaryASTNode<UntypedVarDeclaration>;

template <typename Z>
class TypedVarDeclaration {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using TypedVarDeclASTNode = TertiaryASTNode<TypedVarDeclaration>;

template <typename Z>
class TypeDeclaration {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using TypeDeclASTNode = NoEvalBinaryASTNode<TypeDeclaration>;

// e.g. let decl1 decl2 decl3 in exp1; exp2; exp3; end
template <typename Z>
class LetBlock {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
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
};

using DeclarationASTNode = NoEvalUnaryASTNode<Declaration>;

// a sequence of declarations (the first part of a let)
template <typename Z>
class DeclList {
    public:
        // TODO implement variable declaration
        Z operator() (std::vector<const DeclarationASTNode*> declarations) {
            return -1;
        }
};

using DeclListASTNode = VectorASTNode<DeclList, DeclarationASTNode>;

// e.g. a; b; c; d;
template <typename Z>
class ExprSeq {
    public:
        // TODO implement variable declaration
        Z operator() (std::vector<const ASTNode*> members) {
            return -1;
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
};

using FieldMemberASTNode = NoEvalBinaryASTNode<FieldMember>;

// e.g. a = b, c = d (in a type instantiation inside curly brackets)
template <typename Z>
class FieldList {
    public:
        // TODO implement variable declaration
        Z operator() (std::vector<const FieldMemberASTNode*> members) {
            return -1;
        }
};

using FieldListASTNode = VectorASTNode<FieldList, FieldMemberASTNode>;

// e.g. typename { field1 = val1, field2 = val2 }
template <typename Z>
class TypeInstantiation {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using TypeInstASTNode = NoEvalBinaryASTNode<TypeInstantiation>;

// the thing that goes on the right side of a type declaration
template <typename Z>
class TypeValue {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr child_) {
            return -1;
        }
};

using TypeASTNode = NoEvalUnaryASTNode<TypeValue>;

// one field in a record type, e.g. a : int
template <typename Z>
class RecordField {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using RecordFieldASTNode = NoEvalBinaryASTNode<RecordField>;

// a record type -- i.e. a sequence of RecordFields
template <typename Z>
class RecordType {
    public:
        // TODO implement variable declaration
        Z operator() (std::vector<const RecordFieldASTNode*> fields) {
            return -1;
        }
};

using RecordTypeASTNode = VectorASTNode<RecordType, RecordFieldASTNode>;

// "array of" whatever type
template <typename Z>
class ArrayType {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr child_) {
            return -1;
        }
};

using ArrayTypeASTNode = NoEvalUnaryASTNode<ArrayType>;

// represents lvalue.identifier
template <typename Z>
class DotAccess {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using DotASTNode = NoEvalBinaryASTNode<DotAccess>;

// represents array[index]
template <typename Z>
class IndexAccess {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using IndexASTNode = NoEvalBinaryASTNode<IndexAccess>;

// represents array[index]
template <typename Z>
class ArrayValue {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using ArrayASTNode = TertiaryASTNode<ArrayValue>;

// represents function declerations
template <typename Z>
class UnTypedFuncDecl {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using UnTypedFuncDeclASTNode = TertiaryASTNode<UnTypedFuncDecl>;

// represents function declerations
template <typename Z>
class TypedFuncDecl {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
            return -1;
        }
};

using TypedFuncDeclASTNode = QuaternaryASTNode<TypedFuncDecl>;

// represents function calls
template <typename Z>
class FuncCall {
    public:
        // TODO implement variable declaration
        Z operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }
};

using FuncCallASTNode = NoEvalBinaryASTNode<FuncCall>;

} // namespace

#endif
