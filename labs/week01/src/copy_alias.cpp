#include <iostream>
#include <string_view>
#include <vector>

void print_vector(std::string_view name, const std::vector<int>& values) {
    std::cout << name << " = ";

    for (int value : values) {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}

int main() {
    std::vector<int> a{1, 2, 3};

    auto b = a;   // copy: b is a new, independent vector {1, 2, 3}
    auto& c = a;  // alias: c is just another name for a

    b[0] = 10;    // changes only the copy
    c[1] = 20;    // changes a (because c IS a)

    a.push_back(4);

    // Prediction:
    //   a = 1 20 3 4
    //   b = 10 2 3
    //   c = 1 20 3 4   (always identical to a)
    print_vector("a", a);
    print_vector("b", b);
    print_vector("c", c);
}
