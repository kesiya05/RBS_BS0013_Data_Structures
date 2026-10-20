#include <iostream>

int main() {
    int* p = new int{42};  // dynamically allocate exactly one int; p owns it

    std::cout << "initial = " << *p << '\n';
    *p = 100;              // modify the value through the pointer
    std::cout << "changed = " << *p << '\n';

    delete p;      // release the allocation exactly once
    p = nullptr;   // p now explicitly represents "no object"; *p is never used again

    // Order matters: if we set p = nullptr BEFORE delete, we lose the only
    // address of the allocation, so it can never be freed (memory leak), and
    // `delete nullptr` does nothing.

    return 0;
}
