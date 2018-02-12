/*
 * PtreTree: An implementation of tree class using vectors.
 * Semantics: the ith-node in the tree has and two children at nodes:
 * ((i+1)*2-1) and ((i+1)*2).
 * This implementation imposes an additional constraint that the tree must be
 * fully balanced at all times. An exception is thrown otherwise.
 */

#include "ptretree.hh"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iterator>

namespace tree {

///////////////////////////////////////////////////////////////////////////////
PtreTree::PtreTree(value_t value)
    : Tree()
    {
        flatTree_ = new value_t[1];
        flatTree_[0] = value; 
        in_flattree = 1;   
    }
///////////////////////////////////////////////////////////////////////////////
// This allows the creation of empty trees so that trees with an even number of
// elements can be created. 
PtreTree::PtreTree()
    : Tree()
    {
        flatTree_ = {};
        in_flattree = 0;
    }

///////////////////////////////////////////////////////////////////////////////
unsigned
PtreTree::size() const
{   
    return in_flattree;
}

///////////////////////////////////////////////////////////////////////////////
PtreTree::PtreTree(value_t root, const PtreTree& left, const PtreTree& right)
{
    // if you try to create a compound tree using empty trees
    if (left.size() == 0 and right.size() == 0){
        throw std::runtime_error("this tree already exists -- left and right trees are empty");
    }

    // in order to allow for imbalanced trees, we figure out which subtree is larger
    unsigned max_size;
    if (left.size() != right.size()) {
        imbalanced = true;
    }

    max_size = std::max(left.size(), right.size()); // in case of imbalanced trees

    unsigned array_size = (left.size() + right.size() + 1); // calculate # of elements in new tree
    in_flattree = array_size;
    flatTree_ = new value_t[array_size];
    flatTree_[0] = root;

    unsigned to_copy = 1;
    unsigned track = 0;
    unsigned index = 1;
    value_t* left_ptr = left.flatTree_;
    value_t* right_ptr = right.flatTree_;

    
    for (unsigned copied = 0; copied < max_size; copied += to_copy, to_copy *= 2) {
        while (track < to_copy && left_ptr != left.flatTree_ + left.size()){
            flatTree_[index] = *left_ptr;
            index++;
            left_ptr++;
            track++;
        }
        track = 0;
        while (track < to_copy && right_ptr != right.flatTree_ + right.size()){
            flatTree_[index] = *right_ptr;
            index++;
            right_ptr++;
            track++;
        }
        track = 0;
    }

}

///////////////////////////////////////////////////////////////////////////////
// Scan vector for first ocurrence of value, then build path back up to root

// pathTo does not work for imbalanced trees.
std::string
PtreTree::pathTo(value_t value) const
{
    if (size() == 0) {
        throw std::runtime_error("this is an empty tree. there are no valid paths.");
    }

    if (imbalanced) {
        throw std::runtime_error("pathTo does not work for imbalanced trees.");
    }

	const auto where = std::find(flatTree_, flatTree_ + size(), value);
    if (where == flatTree_ + size()) {
        throw std::runtime_error("Value not found!");
    }

    std::string path = "";
    for (auto idx = where - flatTree_; idx; idx = (idx - 1) / 2) {
        const auto child = idx % 2? 'L' : 'R';
        path.insert(path.begin(), child);
    }
    return path;
}


///////////////////////////////////////////////////////////////////////////////
// Follow path from root to end of string, returning value when done
PtreTree::value_t
PtreTree::getByPath(const std::string& path) const
{
    if (size() == 0) {
        throw std::runtime_error("this is an empty tree. there are no values to get.");
    }
    size_t cur_idx = 0;        // Start by pointing index at root

    for (const auto c : path) {
        const auto right_idx = (cur_idx + 1) * 2;  // Index of right child
        if (right_idx > size()) {
            throw std::runtime_error("Path too long!");
        }
        if (c == 'R') {
            cur_idx = right_idx;
        } else if (c == 'L') {
            cur_idx = right_idx - 1;
        } else {
            throw std::runtime_error("Invalid character in path: " + path);
        }
    }
    return flatTree_[cur_idx];
}


} // namespace

