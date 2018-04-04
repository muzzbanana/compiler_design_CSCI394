#ifndef _TEMP_HH_
#define _TEMP_HH_

class Temp {
    public:
        Temp() { }
        Temp(const std::string specialname) { }
        Temp(const Temp& other) { }

        ~Temp() = default;

        std::string toStr() { return "<temp>"; }

        int index = 0;
}

class Label {
    public:
        Label() { }
        Label(const std::string name) { }
        ~Label() { }

        std::string toStr() { return "<label>"; }
}

#endif
