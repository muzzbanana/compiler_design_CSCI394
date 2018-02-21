/*
 * PtreTree: An implementation of tree class using vectors.
 * Semantics: the ith-node in the tree has and two children at nodes:
 * ((i+1)*2-1) and ((i+1)*2).
 * This implementation imposes an additional constraint that the tree must be
 * fully balanced at all times. An exception is thrown otherwise.
 */

#include "ptretree.hh"

#include <stdexcept>
#include <cassert>

namespace tree {

///////////////////////////////////////////////////////////////////////////////
PtreTree::PtreTree(value_t value)
    : Tree(), value_(value), left_(nullptr), right_(nullptr)
{ }


///////////////////////////////////////////////////////////////////////////////
PtreTree::PtreTree(value_t root, const PtreTree& left, const PtreTree& right)
  : value_(root), left_(left.clone()), right_(right.clone())
{ }


///////////////////////////////////////////////////////////////////////////////
PtreTree::~PtreTree()
{
    if (left_) {
        delete left_;
    }
    if (right_) {
        delete right_;
    }
}

///////////////////////////////////////////////////////////////////////////////
unsigned
PtreTree::size() const
{
    return 1 +
      (left_? left_->size() : 0) +
      (right_? right_->size() : 0);
}


///////////////////////////////////////////////////////////////////////////////
// Recursively search tree for a given value
const PtreTree*
PtreTree::clone() const
{
    auto ret = new PtreTree(value_);
    if (left_) {
        ret->left_ = left_->clone();
    }
    if (right_) {
        ret->right_ = right_->clone();
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Recursively search tree for a given value
bool
PtreTree::find(value_t value) const
{
    return ((value_ == value) ||
            (left_ && left_->find(value)) ||
            (right_ && right_->find(value)));
}

///////////////////////////////////////////////////////////////////////////////
// Recursively search tree, keeping track of path:
std::string
PtreTree::pathTo(value_t value) const
{
    if (value_ == value) {
        return "";
    }

    if (left_ && left_->find(value)) {
        return "L" + left_->pathTo(value);
    } else if (right_ && right_->find(value)) {
        return "R" + right_->pathTo(value);
    } else {
        throw std::runtime_error("Value not found!");
    }
}


///////////////////////////////////////////////////////////////////////////////
// Follow path from root to end of string, returning value when done
PtreTree::value_t
PtreTree::getByPath(const std::string& path) const
{
    const PtreTree* iter = this;

    for (const auto c : path) {
        if (c == 'L') {
            if (iter->left_) {
                iter = iter->left_;
            } else {
                break;
            }
        } else if (c == 'R') {
            if (iter->right_) {
                iter = iter->right_;
            } else {
                break;
            }
        } else {
            throw std::runtime_error("Invalid character in path: " + path);
        }
    }
    return iter->value_;
}


} // namespace

