#include <iostream>

int main() {
    int* p = new int{42};
    delete p;

    // Intentionally invalid for the AddressSanitizer exercise.
    std::cout << *p << '\n';

    // ASan report: heap-use-after-free (READ of size 4).
    // - invalid access: the `std::cout << *p` line in main
    // - allocated by:   `new int{42}` in main
    // - freed by:       `delete p` in main
    //
    // Problem: the int's lifetime ended at `delete p`, but p still holds the
    // old address and is then dereferenced.
    //
    // Correct repair: end the lifetime only after the last use. Read the
    // value first, then release it, then null the pointer:
    //     int* p = new int{42};
    //     std::cout << *p << '\n';  // last use while the object is alive
    //     delete p;
    //     p = nullptr;
    // Even better, avoid manual ownership: a plain `int value = 42;` or
    // `std::unique_ptr<int>` frees the object automatically at the right time.
}
