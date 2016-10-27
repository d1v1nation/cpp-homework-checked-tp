#include <iostream>
#include <limits>
#include "checked_type.h"
//
// Created by cat on 26.10.16.
//

struct userdef_limits {
    static int max() {
        return 7;
    }

    static int min() {
        return -8;
    }
};


int main() {
    typedef checked_type<int> ct;
    ct a = 2;
    ct b = 4;
    ct lm = std::numeric_limits<int>::max();

    std::cout << "Checked, correct: " << a * b << std::endl;

    try {
        std::cout << "Checked, signed multiplication overflow TRY" << std::endl;
        std::cout << (b * lm);
    } catch (const arithm_error& e) {
        std::cout << "error handled." << std::endl;
    }

    try {
        std::cout << "Checked, minint unary minus TRY" << std::endl;
        std::cout << (-(ct(std::numeric_limits<int>::min())));
    } catch (const arithm_error& e) {
        std::cout << "error handled." << std::endl;
    }

    // user-defined limiting through numeric_limits default override:
    typedef checked_type<int, userdef_limits> ct_b; // checked_tp_bounded

    ct_b ab = 2;
    ct_b bb = 4;
    std::cout << "Checked bounded, correct: " << a * a << std::endl;
    try {
        std::cout << "Checked, bounded multiplication overflow TRY" << std::endl;
        std::cout << (ab * bb);
    } catch (const arithm_error& e) {
        std::cout << "error handled." << std::endl;
    }

    try {
        std::cout << "Checked, bounded min unary minus TRY" << std::endl;
        std::cout << (-(ct_b(userdef_limits::min())));
    } catch (const arithm_error& e) {
        std::cout << "error handled." << std::endl;
    }
}