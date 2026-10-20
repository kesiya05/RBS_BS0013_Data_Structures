#include "int_linked_list.hpp"

#include <iostream>
#include <stdexcept>

namespace {
void trace(const char* step, const IntLinkedList& list) {
    std::cout << step << " -> size=" << list.size()
              << ", invariant=" << std::boolalpha << list.check_invariant();
    if (!list.empty()) {
        std::cout << ", front=" << list.front() << ", back=" << list.back();
    }
    std::cout << '\n';
}
}  // namespace

int main() {
    IntLinkedList list;

    std::cout << "Week 4 linked-list lab\n";
    trace("initial (empty)", list);

    // Boundary transitions used while debugging.
    list.push_back(20);                 trace("push_back(20)   empty -> one", list);
    list.push_front(10);                trace("push_front(10)  one -> two", list);
    list.push_back(40);                 trace("push_back(40)", list);
    list.insert_after_first(20, 30);    trace("insert 30 after 20 (middle)", list);
    list.insert_after_first(40, 50);    trace("insert 50 after 40 (after tail)", list);
    list.erase_after_first(40);         trace("erase after 40 (erase tail)", list);
    list.pop_front();                   trace("pop_front", list);
    list.clear();                       trace("clear", list);
    list.clear();                       trace("clear again (already empty)", list);

    list.push_back(7);                  trace("push_back(7)", list);
    list.pop_front();                   trace("pop_front  one -> empty", list);

    try {
        list.pop_front();
    } catch (const std::out_of_range& error) {
        std::cout << "pop_front on empty throws: " << error.what() << '\n';
    }
    return 0;
}
