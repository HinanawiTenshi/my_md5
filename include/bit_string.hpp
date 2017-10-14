#ifndef __BIT_STRING_HPP__
#define __BIT_STRING_HPP__

#include <string>
#include <vector>

class bit_string {
private:
    unsigned int *data;
    unsigned long long size;
public:
    bit_string();
    bit_string(unsigned long long some_size);
    bit_string(const bit_string &obj);
    ~bit_string();

    bool printBinary() const;
    bool printHex() const;
    bool printDec() const;
    unsigned long long getSize() const;
    bool resize(unsigned long long new_size);
    bool setBit(unsigned long long index, int value);
    std::vector<bit_string> split(unsigned long long split_size) const;
    bit_string substr(unsigned long long from, unsigned long long len) const;
    std::string toString();
    unsigned int get_uint(int index) const;

    int operator[](unsigned long long index) const;
    bit_string &operator=(const bit_string &obj);

};

#endif
