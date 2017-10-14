#ifndef __MY_MD5_HPP__
#define __MY_MD5_HPP__

#include "bit_string.hpp"
#include <string>
#include <vector>

class my_md5 {
private:
    bit_string msg;
    bit_string K;

    bool padding();
    std::string hash();

    bool H_md5(unsigned int &a, unsigned int &b, unsigned int &c,
        unsigned int &d, const bit_string &y);
    unsigned int g(unsigned int b, unsigned int c, unsigned int d, int flag);
    unsigned int X(const bit_string &y, int index);
    unsigned int T(int index);
public:
    const static unsigned int IV_A;
    const static unsigned int IV_B;
    const static unsigned int IV_C;
    const static unsigned int IV_D;
    const static unsigned int SHIFT[64];

    my_md5();
    my_md5(std::string m);
    //~my_md5();

    std::string digest();
};

#endif
