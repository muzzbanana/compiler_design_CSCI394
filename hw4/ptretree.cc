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

// implementation of pathTo was inspired by one of the solutions shown in class
// I cannot take credit for it :)

namespace tree {

///////////////////////////////////////////////////////////////////////////////
PtreTree::PtreTree(value_t value)
    : Tree()
    { num_of_elements = 1;
      root_value = value;
      left_subtree = nullptr;
      right_subtree = nullptr;
    }


///////////////////////////////////////////////////////////////////////////////
PtreTree::~PtreTree()
{
    delete left_subtree;

    delete right_subtree;
}

///////////////////////////////////////////////////////////////////////////////
unsigned
PtreTree::size() const
{   
    return num_of_elements;
}

///////////////////////////////////////////////////////////////////////////////
PtreTree::PtreTree(const PtreTree& subtree)
    {
        num_of_elements = subtree.size();
        root_value = subtree.root_value;
        if (subtree.left_subtree != nullptr) {
            left_subtree = new PtreTree(*subtree.left_subtree);
        } else {
            left_subtree = nullptr;
        }
        if (subtree.right_subtree != nullptr) {
            right_subtree = new PtreTree(*subtree.right_subtree);
        } else {
            right_subtree = nullptr;
        }
        
    }

///////////////////////////////////////////////////////////////////////////////
PtreTree::PtreTree(value_t root, const PtreTree& left, const PtreTree& right)
    : Tree()
    {
        num_of_elements = left.size() + right.size() + 1;
        root_value = root;
        left_subtree = new PtreTree(left);
        right_subtree = new PtreTree(right);
    }

///////////////////////////////////////////////////////////////////////////////

bool
PtreTree::isInPath(value_t value) const
{
    return value == root_value ||
            (left_subtree != nullptr && left_subtree->isInPath(value)) ||
            (right_subtree != nullptr && right_subtree->isInPath(value));
}

///////////////////////////////////////////////////////////////////////////////
std::string
PtreTree::pathTo(value_t value) const
{
    if(value == root_value){
        return "";
    }
    else if(left_subtree != nullptr && left_subtree->isInPath(value)){
        return 'L' + left_subtree->pathTo(value);
    }
    else if(right_subtree != nullptr && right_subtree->isInPath(value)){
        return 'R' + right_subtree->pathTo(value);
    }
    else{
        throw std::runtime_error("There is no valid path to this value");
    }
}


///////////////////////////////////////////////////////////////////////////////
// Follow path from root to end of string, returning value when done
PtreTree::value_t
PtreTree::getByPath(const std::string& path) const
{
    if (path.size() == 0){
        return root_value;
    } else if (path[0] == 'L'){
        if (left_subtree == nullptr){
            return root_value;
        } else {
            return left_subtree->getByPath(std::string(path.begin()+1, path.end()));
        }
    } else if (path[0] == 'R'){
        if (right_subtree == nullptr){
            return root_value;
        } else {
            return right_subtree->getByPath(std::string(path.begin()+1, path.end()));
        }
    } else {
        throw std::runtime_error("Invalid character in path: " + path);
    }
}


} // namespace

