#include "my_md5.hpp"
#include <cstdio>
#include <iostream>

const std::string TEST_STR[4] = {
    "",
    "Hello World!",
    "The quick brown fox jumps over the lazy dog",
    "The quick brown fox jumps over the lazy dog."
};
const int TEST_NUM = 4;

int main() {
    printf("============== My_md5 Hash Function ==============\n");
    printf("Some examples:\n\n");
    for (int i = 0; i < TEST_NUM; i++) {
        my_md5 md5(TEST_STR[i]);
        printf("\"%s\"\n%s\n", TEST_STR[i].c_str(), md5.digest().c_str());
    }

    printf("\n");

    while (1) {
        printf("Input a string to hash, end with enter, control-C to exit\n\n");
        std::string str("");
        std::getline(std::cin, str);
        printf("\"%s\"\n%s\n\n", str.c_str(), my_md5(str).digest().c_str());
    }
    return 0;
}
