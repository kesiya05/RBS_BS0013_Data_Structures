#include "int_vector.hpp"

#include <cassert>
#include <cstddef>

// Destructor: IntVector owns data_, so it must release it exactly once.
// delete[] on nullptr is safe, so an empty vector needs no special case.
IntVector::~IntVector() {
    delete[] data_;
}

// Deep copy: allocate NEW storage and copy the elements, so the copy owns
// its own array. Copying only the pointer would make two objects delete the
// same array (double free) and let changes in one appear in the other.
IntVector::IntVector(const IntVector& other)
    : data_(other.capacity_ == 0 ? nullptr : new int[other.capacity_]),
      size_(other.size_),
      capacity_(other.capacity_) {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
    check_invariant();
}

// Representation invariants:
//   0 <= size_ <= capacity_            (size_ is unsigned, so >= 0 holds automatically)
//   capacity_ == 0  <=>  data_ == nullptr
void IntVector::check_invariant() const {
    assert(size_ <= capacity_);
    assert((capacity_ == 0) == (data_ == nullptr));
}

// Geometric growth: 0 -> 1, otherwise double.
// Order: allocate new array, copy old elements, delete old array, then switch.
// size_ is not changed.
void IntVector::grow() {
    const std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;

    int* new_data = new int[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }

    delete[] data_;   // old storage's lifetime ends here
    data_ = new_data;
    capacity_ = new_capacity;

    check_invariant();
}

// Append: grow only when full, write into the first unused slot, then size_++.
// Amortized O(1) because of doubling; O(n) on the appends that trigger grow().
void IntVector::push_back(int value) {
    if (size_ == capacity_) {
        grow();
    }

    data_[size_] = value;
    ++size_;

    check_invariant();
}
