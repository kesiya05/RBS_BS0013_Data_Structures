#include <iostream>

int main() {
    int value = 42;
    int* p = nullptr;

    // State 1: nullptr -> no object. Must not be dereferenced.
    std::cout << "initially null: " << std::boolalpha << (p == nullptr) << '\n';

    // State 2: valid pointer -> live object. Safe to dereference because
    // `value` is still alive (its scope is the whole of main).
    p = &value;
    if (p != nullptr) {
        std::cout << "live value: " << *p << '\n';
    }

    int* dangling = nullptr;
    {
        int temporary = 99;
        dangling = &temporary;
        std::cout << "inside scope: " << *dangling << '\n';  // safe: temporary is alive
    }

    // State 3: dangling -> old address, but lifetime ended.
    // `temporary` was destroyed at the closing brace above. `dangling` still
    // stores the same address, but no object lives there any more, so
    // dereferencing it would be undefined behaviour (it might print 99,
    // garbage, or crash). A pointer is only safe to dereference when it is
    // not null AND the object it points to is still alive. Checking
    // `dangling != nullptr` cannot detect this, because the address is unchanged.
    // Do not add: std::cout << *dangling;

    // Resetting to nullptr turns the dangling pointer into an explicit "no object".
    dangling = nullptr;
    std::cout << "dangling reset to null: " << (dangling == nullptr) << '\n';
}
