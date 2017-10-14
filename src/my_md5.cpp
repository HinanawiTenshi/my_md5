#include "my_md5.hpp"

#include <cstdio>
#include <cmath>

//#define DEBUG

int get_bit(unsigned long long num, int index);
int get_bit(unsigned int num, int index);
unsigned int cls(unsigned int num, int val);
unsigned int endian_byte(unsigned int num);
unsigned long long endian_byte(unsigned long long num);
unsigned long long endian_word(unsigned long long num);

const unsigned int my_md5::IV_A = 0x67452301;
const unsigned int my_md5::IV_B = 0xEFCDAB89;
const unsigned int my_md5::IV_C = 0x98BADCFE;
const unsigned int my_md5::IV_D = 0x10325476;
const unsigned int my_md5::SHIFT[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

my_md5::my_md5() {
    ;
}

my_md5::my_md5(std::string m) {
    msg.resize(m.length() * 8);
    for (int i = 0; i < msg.getSize(); i++)
        msg.setBit(i, ((unsigned int)m[i / 8] & 1 << 7 - i % 8) > 0);
}

std::string my_md5::digest() {
    padding();
    return hash();
}

bool my_md5::padding() {
    unsigned long long total_len;
    int padding_len;

    /* Set total_len and padding_len. */
    if (msg.getSize() % 512 < 449) {
        total_len = (msg.getSize() / 512 + 1) * 512;
        padding_len = 448 - msg.getSize() % 512;
    } else {
        total_len = (msg.getSize() / 512 + 2) * 512;
        padding_len = 512 - msg.getSize() % 448;
    }

    K.resize(total_len);
    /* Set message. */
    for (unsigned long long i = 0; i < msg.getSize(); i++)
        K.setBit(i, msg[i]);
    /* Set padding. */
    for (int i = 0; i < padding_len; i++) {
        if (i == 0)
            K.setBit(msg.getSize() + i, 1);
        else
            K.setBit(msg.getSize() + i, 0);
    }
    /* Set length. */
    unsigned long long len = endian_word(msg.getSize());
    len = endian_byte(len);
    for (int i = 0; i < 64; i++) {
        int val = get_bit(len, i);
        K.setBit(msg.getSize() + padding_len + i, val);
    }

    return true;
}

std::string my_md5::hash() {
    unsigned int cv_a = my_md5::IV_A;
    unsigned int cv_b = my_md5::IV_B;
    unsigned int cv_c = my_md5::IV_C;
    unsigned int cv_d = my_md5::IV_D;

    std::vector<bit_string> split = K.split(512);
    for (int i = 0; i < split.size(); i++) {
        H_md5(cv_a, cv_b, cv_c, cv_d, split[i]);
    }

    cv_a = endian_byte(cv_a);
    cv_b = endian_byte(cv_b);
    cv_c = endian_byte(cv_c);
    cv_d = endian_byte(cv_d);
    bit_string result_str(128);
    for (int i = 0; i < 128; i++) {
        if (i < 32)
            result_str.setBit(i, get_bit(cv_a, i));
        else if (i < 64)
            result_str.setBit(i, get_bit(cv_b, i % 32));
        else if (i < 96)
            result_str.setBit(i, get_bit(cv_c, i % 32));
        else
            result_str.setBit(i, get_bit(cv_d, i % 32));
    }

    return result_str.toString();
}

bool my_md5::H_md5(unsigned int &a, unsigned int &b, unsigned int &c,
                    unsigned int &d, const bit_string &y) {
    // a <- b + ((a + g(b,c,d) + X[k] + T[i]) <<< s)
    unsigned int aa = a, bb = b, cc = c, dd = d;
    for (int i = 0; i < 64; i++) {
#ifdef DEBUG
        printf("[Iteration %d]\t%08X %08X %08X %08X T[%d]=%08X\n", i, aa, bb, cc, dd, i, T(i));
        printf("bb=%08X aa=%08X g=%08X X=%08X cls=%d\n", bb, aa, g(bb, cc, dd, i), X(y, i), SHIFT[i]);
#endif
        unsigned int temp = g(bb, cc, dd, i);
        temp += aa + X(y, i) + T(i);
        aa = dd;
        dd = cc;
        cc = bb;
        bb += cls(temp, SHIFT[i]);
    }
    a += aa;
    b += bb;
    c += cc;
    d += dd;

    return true;
}

unsigned int my_md5::g(unsigned int b, unsigned int c, unsigned int d,
                        int flag) {
    if (flag < 16)
        return (b & c) | (~b & d);
    else if (flag < 32)
        return (b & d) | (c & ~d);
    else if (flag < 48)
        return b ^ c ^ d;
    else
        return c ^ (b | ~d);
}

unsigned int my_md5::X(const bit_string &y, int index) {
    if (index < 16)
        return endian_byte(y.get_uint(index));
    else if (index < 32)
        return endian_byte(y.get_uint((index * 5 + 1) % 16));
    else if (index < 48)
        return endian_byte(y.get_uint((index * 3 + 5) % 16));
    else
        return endian_byte(y.get_uint((index * 7) % 16));
}

unsigned int my_md5::T(int index) {
    return (unsigned int)(pow(2, 32) * fabs(sin(index + 1)));
}

int get_bit(unsigned long long num, int index) {
    return (num & (unsigned long long)1 << 63 - index) > 0;
}

int get_bit(unsigned int num, int index) {
    return (num & (unsigned int)1 << 31 - index) > 0;
}

unsigned int cls(unsigned int num, int val) {
    return (num << val) | (num >> (32 - val));
}

unsigned int endian_byte(unsigned int num) {
    return (num & 0x000000FF) << 24 |
            (num & 0x0000FF00) << 8 |
            (num & 0x00FF0000) >> 8 |
            (num & 0xFF000000) >> 24;
}

unsigned long long endian_byte(unsigned long long num) {
    return (num & 0x00000000000000FF) << 24 |
            (num & 0x000000000000FF00) << 8 |
            (num & 0x0000000000FF0000) >> 8 |
            (num & 0x00000000FF000000) >> 24 |
            (num & 0x000000FF00000000) << 24|
            (num & 0x0000FF0000000000) << 8 |
            (num & 0x00FF000000000000) >> 8 |
            (num & 0xFF00000000000000) >> 24;
}

unsigned long long endian_word(unsigned long long num) {
    return (num & 0x00000000FFFFFFFF) << 32 |
            (num & 0xFFFFFFFF00000000) >> 32;
}
