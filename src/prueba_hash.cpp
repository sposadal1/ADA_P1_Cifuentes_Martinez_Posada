#include <iostream>
#include "third_party/picosha2.h"

int main() {
    std::string s = "abc12";
    std::cout << picosha2::hash256_hex_string(s) << std::endl;
    return 0;
}
