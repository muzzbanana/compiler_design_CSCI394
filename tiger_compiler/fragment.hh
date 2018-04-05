#ifndef _FRAGMENT_HH_
#define _FRAGMENT_HH_

#include <string>
#include <vector>
#include "irtree.hh"

namespace tiger {

class StmtTree;

/* mock classes */
class Frame {
    public:
        Frame() { }
        virtual ~Frame() { }
};

class Fragment {
    protected:
        enum class FragType {
            PROC,
            DATA,
        };

        Fragment(FragType type) : typeID(type) { };

        const FragType typeID;

    public:
        bool isProc() const;
        bool isData() const;
};

inline bool Fragment::isProc() const {
    return typeID == Fragment::FragType::PROC;
}

inline bool Fragment::isData() const {
    return typeID == Fragment::FragType::DATA;
}

class ProcFragment : public Fragment {
    public:
        ProcFragment(StmtTree *stm, Frame *frame);

        virtual std::string toStr();

        StmtTree *getStm();

        Frame *getFrame();

    private:
        StmtTree *tree;
        Frame *frame;
};

class DataFragment : public Fragment {
    public:
        DataFragment(const std::string &s);
        virtual std::string toString();

    private:
        std::string data;
};

}//namespace

#endif
