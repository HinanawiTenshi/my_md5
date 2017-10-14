#include "bit_string.hpp"

#include <cstring>
#include <cstdio>

bit_string::bit_string() {
    data = 0;
    size = 0;
}

bit_string::bit_string(unsigned long long some_size) {
    size = some_size;
    data = new unsigned int[size / 32 + 1];
    memset(data, 0, size / 32 + 1);
}

bit_string::bit_string(const bit_string &obj) {
    size = obj.size;
    data = new unsigned int[size / 32 + 1];

    for (int i = 0; i < size; i++)
        setBit(i, obj[i]);
}

bit_string::~bit_string() {
    delete[] data;
}

unsigned long long bit_string::getSize() const {
    return size;
}

bool bit_string::resize(unsigned long long new_size) {
    delete[] data;
    size = new_size;
    data = new unsigned int[size / 32 + 1];
    memset(data, 0, size / 32 + 1);

    return true;
}

bool bit_string::setBit(unsigned long long index, int value) {
    if (value)
        data[index / 32] |= 1 << 31 - index % 32;
    else
        data[index / 32] &= ~(1 << 31 - index % 32);

    return true;
}

int bit_string::operator[](unsigned long long index) const {
    return (data[index / 32] & 1 << 31 - index % 32) > 0;
}

bit_string &bit_string::operator=(const bit_string &obj) {
    delete[] data;
    size = obj.size;
    data = new unsigned int[size / 32 + 1];

    for (int i = 0; i < size; i++)
        setBit(i, obj[i]);

    return *this;
}

std::vector<bit_string> bit_string::split(unsigned long long split_size) const {
    if (size % split_size != 0 || size == 0)
        return std::vector<bit_string>();

    std::vector<bit_string> result;
    for (int i = 0; i < size / split_size; i++)
        result.push_back(this->substr(split_size * i, split_size));

    return result;
}

bit_string bit_string::substr(unsigned long long from, unsigned long long len) const {
    bit_string result(len);
    for (unsigned long long i = 0; i < len; i++) {;
        result.setBit(i, (*this)[i + from]);
    }
    return result;
}

std::string bit_string::toString() {
    std::string result = "";
    for (int i = 0; i < size / 32; i++) {
        char buf[8];
        sprintf(buf, "%08x", data[i]);
        result += buf;
    }
    return result;
}

unsigned int bit_string::get_uint(int index) const {
    if (index < 0 || index >= size / 32)
        return 0;
    return data[index];
}

bool bit_string::printBinary() const {
    for (int i = 0; i < size; i++)
        printf("%d", (*this)[i]);
    printf("\n");

    return true;
}

bool bit_string::printHex() const {
    printf("0x");
    for (int i = 0; i < size / 32 - 1; i++)
        printf("%08X ", data[i]);
    printf("%08X\n", data[size / 32 - 1]);

    return true;
}

bool bit_string::printDec() const {
    for (int i = 0; i < size / 32 - 1; i++)
        printf("%u ", data[i]);
    printf("%u\n", data[size / 32 - 1]);

    return true;
}
