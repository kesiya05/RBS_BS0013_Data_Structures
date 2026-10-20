#include <iostream>

int main() {
    int a = 10;
    int b = 20;

    int* p = &a;
    int* q = p;

    // Trace (completed before running):
    // step                  a    b    p points to    q points to
    // after declarations    10   20   a              a

    *q += 5;
    // after *q += 5         15   20   a              a

    p = &b;
    // after p = &b          15   20   b              a

    *p += 7;
    // after *p += 7         15   27   b              a

    // Why reassigning p did not reassign q:
    // `int* q = p;` copied the address stored in p (the address of a) into q.
    // After that, p and q are two independent pointer variables. `p = &b;`
    // only overwrites the address stored in p; q still holds a's address.
    // So *p is b (27) and *q is still a (15).

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "*p = " << *p << '\n';
    std::cout << "*q = " << *q << '\n';
}
