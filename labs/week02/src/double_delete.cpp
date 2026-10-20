int main() {
    int* p = new int{42};
    int* q = p;

    delete p;

    // Intentionally invalid for the AddressSanitizer exercise.
    delete q;

    // ASan report: attempting double-free.
    // - second free:  `delete q` in main
    // - first free:   `delete p` in main
    // - allocated by: `new int{42}` in main
    //
    // Why two pointer variables do not imply two allocations:
    // `int* q = p;` copies only the address, not the int. There is still just
    // ONE object created by ONE `new`, so it must be released by ONE `delete`.
    // After `delete p`, q is dangling, and `delete q` frees the same block again.
    //
    // Correct repair: decide that exactly one pointer is the owner (here p).
    // q is only a non-owning observer and must never delete:
    //     int* p = new int{42};   // owner
    //     int* q = p;             // non-owning alias
    //     delete p;               // the single release
    //     p = nullptr;
    //     q = nullptr;            // q must not be used after the release
    // Or use std::unique_ptr<int>, which makes single ownership explicit.
}
