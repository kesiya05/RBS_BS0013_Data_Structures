#include <iostream>

// Pass by value: x is a separate copy, so ++x only changes the copy.
void increment_copy(int x) {
    ++x;
}

// Pass by reference: x is another name for the caller's variable.
void increment(int& x) {
    ++x;
}

int main() {
    int value = 10;

    increment_copy(value);
    std::cout << "After pass by value:     " << value << '\n';  // prediction: 10

    increment(value);
    std::cout << "After pass by reference: " << value << '\n';  // prediction: 11
}
