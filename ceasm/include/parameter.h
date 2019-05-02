#ifndef EASM_PARAMETER_H
#define EASM_PARAMETER_H

#include <string>
#include <ostream>

enum class RegSize {
    QWORD = 4, DWORD = 3, WORD = 2, BYTE = 1
};

struct Parameter {
    virtual void display(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Parameter& instance);
};

struct RegParam : public Parameter {
    RegSize type;
    int index;

    RegParam(RegSize type, int index)
        : type(type), index(index) {}
    virtual void display(std::ostream& out) const;
};

struct LiteralParam : public Parameter {
    uint32_t value;
    LiteralParam(uint32_t value)
        : value(value) {}
    virtual void display(std::ostream& out) const;
};

struct LabelParam : public Parameter {
    std::string name;
    LabelParam(std::string name) 
        : name(name) {}
    virtual void display(std::ostream& out) const;
};

inline std::ostream& operator<<(std::ostream& out, const Parameter& inst) {
    inst.display(out);
    return out;
}


#endif