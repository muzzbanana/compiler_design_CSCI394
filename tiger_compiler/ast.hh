/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/RCS/fb3-1.h,v 2.1 2009/11/08 02:53:18 johnl Exp $
 */
/*
 * Declarations for a calculator fb3-1
 */

#include <functional>
#include <string>
#include <cmath>


/* interface to the lexer */
void yyerror(char *s, ...);

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
  const ASTptr child_;
  const std::string rep_;  // String representation of node
};

// Example usage of UnaryASTNode:
using UnaryMinusASTNode = UnaryASTNode<std::negate>;

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
   : ASTNode(), rep_(rep), left_(left), right_(right)
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
    return  "(" + left_->toStr() +
      " " + rep_ + " " +
      right_->toStr() + ")";
  }

 private:
  const ASTptr left_, right_;
  const std::string rep_;  // String representation of node
};

using PlusASTNode = BinaryASTNode<std::plus>;
using MinusASTNode = BinaryASTNode<std::minus>;
using MultASTNode = BinaryASTNode<std::multiplies>;
using DivASTNode = BinaryASTNode<std::divides>;

} // namespace
