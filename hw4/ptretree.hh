/*
 * PtreTree.hh: API for a simple, non-mutating binary tree class.
 * A tree cannot be empty: it always has at least a root with a value.
 * See tree.hh for virtual API documentation.
 * Written by Eitan Frachtenberg.
 */

#pragma once

#include <string>
#include <memory>

#include "tree.hh"

namespace tree {

class PtreTree : public Tree {
  public:
    PtreTree(value_t value);
    ~PtreTree();
    PtreTree(value_t newroot, const PtreTree& left, const PtreTree& right);

    virtual unsigned size() const override;

    virtual std::string pathTo(value_t value) const override;

    value_t getByPath(const std::string& path) const override;

    const PtreTree* clone() const;  // Make a deep copy

  private:
    bool find(value_t) const;  // Is value in my tree?

    value_t value_;
    const PtreTree* left_;
    const PtreTree* right_;
};

} // namespace
