#include "int_linked_list.hpp"

#include <stdexcept>

IntLinkedList::~IntLinkedList() {
    clear();
}

// front/back: Theta(1). Empty list -> std::out_of_range (required policy).
int& IntLinkedList::front() {
    if (empty()) {
        throw std::out_of_range("front() on empty list");
    }
    return head_->value;
}

const int& IntLinkedList::front() const {
    if (empty()) {
        throw std::out_of_range("front() on empty list");
    }
    return head_->value;
}

int& IntLinkedList::back() {
    if (empty()) {
        throw std::out_of_range("back() on empty list");
    }
    return tail_->value;  // cached tail_ makes this Theta(1)
}

const int& IntLinkedList::back() const {
    if (empty()) {
        throw std::out_of_range("back() on empty list");
    }
    return tail_->value;
}

// push_front: new node points to old head, then becomes head. Theta(1).
// Boundary: empty -> one node, so the new node is also the tail.
void IntLinkedList::push_front(int value) {
    Node* node = new Node{value, head_};
    head_ = node;
    if (tail_ == nullptr) {
        tail_ = node;
    }
    ++size_;
}

// push_back: link after the cached tail. Theta(1) thanks to tail_.
// Boundary: empty -> one node, so the new node is also the head.
void IntLinkedList::push_back(int value) {
    Node* node = new Node{value, nullptr};
    if (tail_ == nullptr) {
        head_ = node;
    } else {
        tail_->next = node;
    }
    tail_ = node;
    ++size_;
}

// pop_front: save the successor BEFORE deleting the old head. Theta(1).
// Boundary: one node -> empty, so tail_ must become nullptr too.
void IntLinkedList::pop_front() {
    if (empty()) {
        throw std::out_of_range("pop_front() on empty list");
    }
    Node* old_head = head_;
    head_ = old_head->next;
    delete old_head;
    --size_;
    if (head_ == nullptr) {
        tail_ = nullptr;
    }
}

// contains: linear traversal from head_. Theta(n) worst case.
bool IntLinkedList::contains(int value) const noexcept {
    for (const Node* current = head_; current != nullptr; current = current->next) {
        if (current->value == value) {
            return true;
        }
    }
    return false;
}

// Navigation to target: Theta(n) worst case. Rewiring after it: Theta(1).
bool IntLinkedList::insert_after_first(int target, int value) {
    Node* current = head_;
    while (current != nullptr && current->value != target) {
        current = current->next;
    }
    if (current == nullptr) {
        return false;  // target absent
    }

    Node* node = new Node{value, current->next};  // new node takes over the successor
    current->next = node;
    if (current == tail_) {
        tail_ = node;  // inserted after the tail -> new tail
    }
    ++size_;
    return true;
}

// Removes the node right after the first target. Theta(n) worst case (navigation).
bool IntLinkedList::erase_after_first(int target) {
    Node* current = head_;
    while (current != nullptr && current->value != target) {
        current = current->next;
    }
    if (current == nullptr || current->next == nullptr) {
        return false;  // target absent, or target is the tail (no successor)
    }

    Node* removed = current->next;
    current->next = removed->next;  // read removed->next BEFORE delete
    if (removed == tail_) {
        tail_ = current;  // erased the tail -> predecessor becomes tail
    }
    delete removed;
    --size_;
    return true;
}

// Releases every node exactly once. Safe on an already-empty list. Theta(n).
void IntLinkedList::clear() noexcept {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;  // save before deleting
        delete current;
        current = next;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
}

// Checks metadata consistency, tail_->next, node count, final node == tail_.
// Stops after size_ steps, so an accidental cycle cannot loop forever.
bool IntLinkedList::check_invariant() const noexcept {
    if (size_ == 0) {
        return head_ == nullptr && tail_ == nullptr;
    }
    if (head_ == nullptr || tail_ == nullptr || tail_->next != nullptr) {
        return false;
    }

    const Node* current = head_;
    const Node* last = nullptr;
    std::size_t count = 0;
    while (current != nullptr) {
        ++count;
        if (count > size_) {
            return false;  // more nodes than size_, or a cycle
        }
        last = current;
        current = current->next;
    }
    return count == size_ && last == tail_;
}
