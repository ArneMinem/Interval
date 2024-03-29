// TD 1 : Interval and arithmetic operations
// 11/03/2024
// Author: JACOBS Arne

// B. Programming a simple interval library

// 5. In C++, implement a simple Interval class.

#include "Interval.hpp"

int main() {
    Interval i1(1, 5);
    Interval i2(3, 4);
    Interval i3 = max(i1, i2);
    std::cout << i1.get_lb() << std::endl;
    std::cout << i1.get_ub() << std::endl;
    std::cout << i1.is_empty() << std::endl;
    std::cout << i1.is_unbounded() << std::endl;
    std::cout << i1.is_singleton() << std::endl;
    std::cout << i1.contains(1.5) << std::endl;
    std::cout << i1.contains(i2) << std::endl;
    std::cout << i3.get_lb() << std::endl;
    std::cout << i3.get_ub() << std::endl;
    return 0;
}