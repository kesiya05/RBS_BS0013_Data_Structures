#include <cstddef>
#include <iostream>

struct Node {
    int value;
    Node* next = nullptr;
};

// Start at head and follow next until nullptr: Theta(n) time, Theta(1) extra space.
void print_chain(const Node* head) {
    const Node* current = head;
    while (current != nullptr) {
        std::cout << current->value << ' ';
        current = current->next;
    }
}

// Extension 1: counts nodes without modifying the chain.
// Time Theta(n), auxiliary space Theta(1) (one pointer and one counter).
std::size_t chain_length(const Node* head) {
    std::size_t count = 0;
    for (const Node* current = head; current != nullptr; current = current->next) {
        ++count;
    }
    return count;
}

// Extension 2: returns the first node holding target, or nullptr if absent.
// Best case Theta(1) (target is at head); worst case Theta(n) (target is last or absent).
const Node* find_value(const Node* head, int target) {
    for (const Node* current = head; current != nullptr; current = current->next) {
        if (current->value == target) {
            return current;
        }
    }
    return nullptr;
}

int main() {
    Node a{10};
    Node b{20};
    Node c{30};
    Node d{25};

    a.next = &b;
    b.next = &c;

    // Task 6A: temporarily rewire so traversal from a is 10 -> 30 -> null.
    a.next = &c;
    // b still exists: it is a local variable of main, so its lifetime lasts
    // until main ends. Changing a.next only changed a link, so b is now
    // unreachable from a, but it is still alive (and b.next still points to c).
    // Existence (lifetime) and reachability (from a given head) are different.

    // Task 6B: restore and then insert d so the final chain is:
    // 10 -> 20 -> 25 -> 30 -> null
    a.next = &b;   // restore 10 -> 20 -> 30
    d.next = &c;   // new node points to the node after the insertion point first
    b.next = &d;   // then link the previous node to the new node
    // Only next pointers changed; no node values were copied or modified.
    // Order matters: setting d.next before b.next means we never lose access to c.
    // c.next is still nullptr, so the chain ends correctly.

    print_chain(&a);
    std::cout << '\n';

    // Extension checks (written to stderr so the required stdout stays "10 20 25 30 ").
    std::cerr << "length = " << chain_length(&a) << '\n';
    std::cerr << "find 25: " << (find_value(&a, 25) == &d ? "found" : "missing") << '\n';
    std::cerr << "find 99: " << (find_value(&a, 99) == nullptr ? "not found" : "found") << '\n';
}
