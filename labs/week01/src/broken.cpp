#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{4, 8, 15, 16, 23, 42};  // fix 1: missing semicolon

    for (int value : values) {        // fix 2: loop over `values`, not `value`
        std::cout << value << ' ';    // fix 3: `cout` needs the std:: prefix
    }

    std::cout << '\n';                // fix 4: missing semicolon
    return 0;                         // fix 5: main returns int, not a string "0"
}
