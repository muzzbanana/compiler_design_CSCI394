#include <ostream>
#include <string>
#include "tree.hh"

namespace tree {

class PtreTree : public Tree {
	public:
		PtreTree(value_t value);
        PtreTree(const PtreTree& subtree);
		~PtreTree();
		PtreTree(value_t newroot, const PtreTree& left, const PtreTree& right);

		virtual unsigned size() const override;

    	virtual std::string pathTo(value_t value) const override;

    	value_t getByPath(const std::string& path) const override;


    private:
    	value_t num_of_elements;
        value_t root_value;
        PtreTree* left_subtree;
        PtreTree* right_subtree;
        bool isInPath(value_t value) const;
};

}
