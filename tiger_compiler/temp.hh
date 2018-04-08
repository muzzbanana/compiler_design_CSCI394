#include <string>
#ifndef _TEMP_HH_
#define _TEMP_HH_

class Temp;

typedef std::vector<Temp*> TempList;

class Temp {
    public:
        Temp() { }
        Temp(const std::string specialname) { }
        Temp(const Temp& other) { }

        ~Temp() = default;

        std::string toStr() { return "<temp>"; }

        int index = -1;

    private:
        int offset;
        std::string specialname;
        Temp& other;
};

class Label {
    public:
        Label() { }
        Label(const std::string name) { }
        ~Label() { }

        std::string toStr() { return "<label>"; }

        int index = -1

    private:
        std::string name;

};






#endif
