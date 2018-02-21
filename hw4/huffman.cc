#include <vector>
#include <iostream>
#include <exception>
#include <queue>
#include <string>
#include <algorithm>
#include <typeinfo>

#include "huffman.hh"
#include "ptretree.hh"

namespace huffman {

struct Huffman::Impl {

    std::vector<std::pair<Huffman::symbol_t,unsigned>> char_freqs;
    symbol_t EOFF = symbol_t(256);  // pseudo end-of-file symbol
    unsigned non_leaf_value = 257;  // values for non-leaf nodes

    Impl() {
        for (int i=0; i<256; ++i){
            char_freqs.push_back(std::make_pair(symbol_t(i),0));
        }
        char_freqs.push_back(std::make_pair(EOFF,1));
    }

    // Compares the frequencies. This is used as the comparator when creating
    // the priority queue which is used in the construction of the tree.
    class Compare {
        public:
            bool operator()(std::pair<tree::PtreTree*, unsigned> &a,const std::pair<tree::PtreTree*, unsigned> &b)
        {
            return (a.second > b.second);
        }
    };


    tree::PtreTree* constructTree()
        {
        std::priority_queue<std::pair<tree::PtreTree*, unsigned>, std::vector<
            std::pair<tree::PtreTree*, unsigned>>, Impl::Compare> tree_vector;

        // this loop creates a vector of pairs of trees and frequencies. the trees
        // created only have one element (they are all leaves)
        for (std::vector<std::pair<Huffman::symbol_t,unsigned> >::iterator where = char_freqs.begin(); 
            where != char_freqs.end(); ++where) {
            tree_vector.push(std::make_pair(new tree::PtreTree(symbol_t(where->first)),where->second));
            
        }

        // using the created vector of leaves, this builds the tree by getting the
        // top 2 pairs and creating their parent node. all parent nodes have value
        // non_leaf_value
        tree::PtreTree *root_ptr = nullptr;
        while (tree_vector.size() > 1) {
            tree::PtreTree *l, *r;
            unsigned lv, rv;
            l = tree_vector.top().first;
            lv = tree_vector.top().second;
            tree_vector.pop();
            r = tree_vector.top().first;
            rv = tree_vector.top().second;
            tree_vector.pop();
            root_ptr = new tree::PtreTree(non_leaf_value, *l, *r);
            tree_vector.push(std::make_pair(root_ptr, lv+rv));

        }
        return root_ptr;
    }

    // createEncoding changes the string of L's and R's into 0's and 1's
    // this makes it possible to use PtreTree's pathTo method
    Huffman::encoding_t createEncoding(std::string path){ 
        Huffman::encoding_t in_code;
        for(std::string::iterator where = path.begin(); where != path.end(); where++){
            if(*where == 'L'){
                in_code.push_back(bit_t::ZERO);
            } else {
                in_code.push_back(bit_t::ONE);
            }
        }
        return in_code;
    }

    // convertTo String does the opposite of createEncoding so to use PtreTree's
    // getByPath method
    std::string convertToString(encoding_t code){
        std::string path;
        for(enc_iter_t where = code.begin(); where != code.end(); ++where){
            if(*where == bit_t::ZERO){
                path.push_back('L');
            } else {
                path.push_back('R');
            }
        }
        return path;
    }

    // this is called by the decode method. it returns a value_t using 
    // convertToString and getByPath
    tree::Tree::value_t decodeHelper(enc_iter_t begin, enc_iter_t end){
        encoding_t code;

        while (begin != end){
            code.push_back(*begin);
            ++begin;
        }

        std::string path = convertToString(code);
        return constructTree()->getByPath(path);
        }


};


Huffman::Huffman() noexcept:
    pImpl_(new Impl)
    {}

Huffman::~Huffman() noexcept
    {}


// searches through the vector of character-frequency pairs and increments the
// frequency for the given character
void
Huffman::incFreq(symbol_t symbol){
    const auto where = std::find_if(pImpl_->char_freqs.begin(), pImpl_->char_freqs.end(),
        [&](std::pair<symbol_t, unsigned> const & check) {return check.first == symbol;});
    if (where != pImpl_->char_freqs.end()){
        (where->second)++;
    } else {
        std::runtime_error("the symbol was not found");
    }    
}

Huffman::encoding_t
Huffman::encode(symbol_t symbol) const {
    std::string path = pImpl_->constructTree()->pathTo(symbol);
    return pImpl_->createEncoding(path);
}

Huffman::symbol_t
Huffman::decode(enc_iter_t& begin, const enc_iter_t& end) const noexcept(false){

    if (begin > end){
        throw std::runtime_error("begin is greater than end");
    }

    tree::Tree::value_t symbol = pImpl_->decodeHelper(begin,end);

    if(symbol == pImpl_->EOFF){
        begin = end;
        return pImpl_->EOFF;  
    } else if(symbol < 256){
        unsigned dist_from_root = pImpl_->constructTree()->pathTo(symbol).size();
        begin += dist_from_root;
        return symbol_t(symbol);

    } else{
        throw std::runtime_error("decoding did not work");
    }

}

Huffman::encoding_t
Huffman::eofCode() const{
    std::string path = pImpl_->constructTree()->pathTo(pImpl_->EOFF);
    return pImpl_->createEncoding(path);
}

} // end of namespace

