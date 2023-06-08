#include <iostream>
#include "instruction.h"

int main() {
    auto access_seq = gen_access_seq(320, 6);
    for (auto i: access_seq) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    return 0;
}
