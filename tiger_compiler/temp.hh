#include <string>
#include <sstream>
#include <vector>
#ifndef _TEMP_HH_
#define _TEMP_HH_

class Temp;

typedef std::vector<Temp*> TempList;

class Temp {
    public:
        Temp() {
            ntemps ++;
            index = ntemps;
        }

        ~Temp() = default;

        std::string toStr() {
            std::stringstream ss;
            ss << "t";
            ss << index;
            return ss.str();
        }

        int index = -1;

    private:
        int offset;

        static int ntemps;
        //Temp& other;
};

class Label {
    public:
        Label() { nlabels ++; index = nlabels; }
        Label(const std::string name) { nlabels ++; index = nlabels; name_ = name; }
        ~Label() { }

        std::string toStr() const {
            std::stringstream ss;
            if (name_.length() == 0) {
                ss << "L";
            } else {
                ss << name_;
            }
            ss << index;
            return ss.str();
        }

        int index = -1;

    private:
        std::string name_ = "";

        static int nlabels;

};




#endif
