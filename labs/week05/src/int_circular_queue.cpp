#include "int_circular_queue.hpp"

#include <cassert>
#include <stdexcept>

// Representation: data_ is a fixed ring buffer, front_ is the physical index of
// the oldest element, size_ is how many logical elements are stored.
// Logical element i lives at physical index (front_ + i) % capacity().

IntCircularQueue::IntCircularQueue(std::size_t capacity)
    : data_(capacity) {
    assert(capacity > 0);
    check_invariant();
}

bool IntCircularQueue::empty() const {
    return size_ == 0;
}

bool IntCircularQueue::full() const {
    return size_ == data_.size();
}

std::size_t IntCircularQueue::size() const {
    return size_;
}

std::size_t IntCircularQueue::capacity() const {
    return data_.size();
}

// front(): the oldest element, stored directly at front_. Theta(1).
int& IntCircularQueue::front() {
    if (empty()) {
        throw std::out_of_range("front() on empty queue");
    }
    return data_[front_];
}

const int& IntCircularQueue::front() const {
    if (empty()) {
        throw std::out_of_range("front() on empty queue");
    }
    return data_[front_];
}

// back(): the newest element = logical position size_ - 1. Theta(1).
int& IntCircularQueue::back() {
    if (empty()) {
        throw std::out_of_range("back() on empty queue");
    }
    return data_[physical_index(size_ - 1)];
}

const int& IntCircularQueue::back() const {
    if (empty()) {
        throw std::out_of_range("back() on empty queue");
    }
    return data_[physical_index(size_ - 1)];
}

// enqueue: write into the first free slot after the last element,
// wrapping past the end of the array with %. Theta(1) worst case: no growth, no shifting.
void IntCircularQueue::enqueue(int value) {
    if (full()) {
        throw std::overflow_error("enqueue() on full queue");
    }
    data_[(front_ + size_) % capacity()] = value;
    ++size_;
    check_invariant();
}

// dequeue: just move front_ forward one slot (wrapping) and shrink size_.
// No elements are shifted, so it is Theta(1) worst case. The old value stays
// in the array but is no longer part of the logical queue.
void IntCircularQueue::dequeue() {
    if (empty()) {
        throw std::underflow_error("dequeue() on empty queue");
    }
    front_ = (front_ + 1) % capacity();
    --size_;
    check_invariant();
}

// clear: logically empty the queue; capacity stays the same. Theta(1) for ints.
void IntCircularQueue::clear() {
    front_ = 0;
    size_ = 0;
    check_invariant();
}

// Maps a logical position (0 = front) to a physical array index.
// Example: capacity 5, front_ = 3, size_ = 4
//   logical 0 -> (3+0)%5 = 3
//   logical 1 -> (3+1)%5 = 4
//   logical 2 -> (3+2)%5 = 0   <- wraps around the end of the array
//   logical 3 -> (3+3)%5 = 1
std::size_t IntCircularQueue::physical_index(std::size_t logical_index) const {
    assert(logical_index < size_);
    return (front_ + logical_index) % capacity();
}

// Invariant: capacity > 0, 0 <= size_ <= capacity, 0 <= front_ < capacity.
// (size_ and front_ are unsigned, so the >= 0 parts hold automatically.)
void IntCircularQueue::check_invariant() const {
    assert(!data_.empty());
    assert(front_ < data_.size());
    assert(size_ <= data_.size());
}
