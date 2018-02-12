#include <ostream>
#include <string>
#include "tree.hh"

namespace tree {

class PtreTree : public Tree {
	public:
		PtreTree();
		PtreTree(value_t value);
		~PtreTree();
		PtreTree(value_t newroot, const PtreTree& left, const PtreTree& right);
		// PtreTree(value_t newroot, const PtreTree& child_tree);

		virtual unsigned size() const override;

    	virtual std::string pathTo(value_t value) const override;

    	value_t getByPath(const std::string& path) const override;

    private:
    	value_t* flatTree_;
    	value_t in_flattree; // number of elements in flatTree_, to keep track of size
    	bool imbalanced = false; // indicates where tree is imbalanced or not
};

}