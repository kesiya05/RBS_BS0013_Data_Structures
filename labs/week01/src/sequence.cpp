#include <iostream>
#include <stdexcept>
#include <vector>

void print_values(const std::vector<int>& values) {
    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

int sum(const std::vector<int>& values) {
    int total = 0;
    for (int value : values) {
        total += value;
    }
    return total;
}

// Must take a (non-const) reference. With `std::vector<int> values` the function
// would change its own copy, and the vector in main() would stay unchanged.
void add_to_all(std::vector<int>& values, int amount) {
    for (int& value : values) {  // int& so we change the real element, not a copy
        value += amount;
    }
}

// Extension A. An empty vector has no minimum/maximum, so throw an error.
int minimum(const std::vector<int>& values) {
    if (values.empty()) {
        throw std::invalid_argument("minimum() called on an empty vector");
    }
    int smallest = values[0];
    for (int value : values) {
        if (value < smallest) {
            smallest = value;
        }
    }
    return smallest;
}

int maximum(const std::vector<int>& values) {
    if (values.empty()) {
        throw std::invalid_argument("maximum() called on an empty vector");
    }
    int largest = values[0];
    for (int value : values) {
        if (value > largest) {
            largest = value;
        }
    }
    return largest;
}

// Extension B. Looks at every element once -> O(n).
int count_value(const std::vector<int>& values, int target) {
    int count = 0;
    for (int value : values) {
        if (value == target) {
            ++count;
        }
    }
    return count;
}

int main() {
    std::vector<int> values{12, 7, 18, 4, 21, 9};

    std::cout << "Original: ";
    print_values(values);
    std::cout << "Sum: " << sum(values) << '\n';

    add_to_all(values, 5);

    std::cout << "Modified: ";
    print_values(values);
    std::cout << "New sum: " << sum(values) << '\n';

    // Extension A
    std::cout << "\nMinimum: " << minimum(values) << '\n';
    std::cout << "Maximum: " << maximum(values) << '\n';
    try {
        minimum(std::vector<int>{});
    } catch (const std::invalid_argument& error) {
        std::cout << "Empty vector: " << error.what() << '\n';
    }

    // Extension B
    std::vector<int> repeated{3, 7, 3, 10, 3};
    std::cout << "\nCount of 3 in {3 7 3 10 3}: " << count_value(repeated, 3) << '\n';

    // Extension C: a reference has the same address as the original variable.
    int a = 10;
    int& b = a;
    std::cout << "\n&a = " << &a << '\n';
    std::cout << "&b = " << &b << '\n';
}
