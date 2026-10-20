#include "int_hash_map.hpp"

#include <cassert>
#include <stdexcept>

// Open addressing with linear probing.
// Probe sequence for key k: bucket(k), bucket(k)+1, ... (mod capacity), at most capacity() slots.
//   Empty    -> key cannot be further along this sequence, lookup may stop
//   Occupied -> compare the stored key
//   Deleted  -> tombstone: no live entry here, but lookup must keep going
// Core invariant: every live key is reachable by following its probe sequence
// from its preferred bucket.

IntHashMap::IntHashMap(std::size_t capacity) : slots_(capacity) { assert(capacity > 0); }

double IntHashMap::load_factor() const noexcept {
    return static_cast<double>(size_) / static_cast<double>(capacity());
}

// Preferred bucket. Example: capacity 7 -> 10, 17, 24 all map to 3 (they collide).
std::size_t IntHashMap::bucket(int key) const {
    assert(key >= 0);
    return static_cast<std::size_t>(key) % capacity();
}

// Returns the slot index holding key, or capacity() if the key is absent.
// Stops at the first Empty slot or after inspecting every slot once (so a full
// table of tombstones/other keys cannot loop forever).
std::size_t IntHashMap::find_index(int key) const {
    assert(key >= 0);
    const std::size_t start = bucket(key);
    for (std::size_t step = 0; step < capacity(); ++step) {
        const std::size_t i = (start + step) % capacity();  // % gives wrap-around 6 -> 0 -> 1
        const HashSlot& slot = slots_[i];
        if (slot.state == SlotState::Empty) {
            return capacity();  // key would have been placed here or earlier
        }
        if (slot.state == SlotState::Occupied && slot.key == key) {
            return i;
        }
        // Deleted, or Occupied by a different key: keep probing.
    }
    return capacity();
}

bool IntHashMap::contains(int key) const { return find(key) != nullptr; }

int* IntHashMap::find(int key) {
    const std::size_t i = find_index(key);
    return i == capacity() ? nullptr : &slots_[i].value;
}

const int* IntHashMap::find(int key) const {
    const std::size_t i = find_index(key);
    return i == capacity() ? nullptr : &slots_[i].value;
}

// Returns true for a new key, false when an existing key is updated.
// A tombstone is remembered as a reusable position, but probing CONTINUES:
// the same key may still exist later in the sequence (it was inserted before
// the tombstone was created), and inserting at the tombstone would duplicate it.
bool IntHashMap::insert_or_assign(int key, int value) {
    assert(key >= 0);
    const std::size_t start = bucket(key);
    std::size_t first_tombstone = capacity();  // capacity() = none found yet

    for (std::size_t step = 0; step < capacity(); ++step) {
        const std::size_t i = (start + step) % capacity();
        HashSlot& slot = slots_[i];

        if (slot.state == SlotState::Occupied) {
            if (slot.key == key) {
                slot.value = value;  // update, size unchanged
                return false;
            }
        } else if (slot.state == SlotState::Deleted) {
            if (first_tombstone == capacity()) {
                first_tombstone = i;
            }
        } else {  // Empty: key is definitely absent
            const std::size_t target = (first_tombstone != capacity()) ? first_tombstone : i;
            slots_[target] = HashSlot{key, value, SlotState::Occupied};
            ++size_;
            return true;
        }
    }

    // Whole table inspected, key absent, no Empty slot. Reuse a tombstone if any.
    if (first_tombstone != capacity()) {
        slots_[first_tombstone] = HashSlot{key, value, SlotState::Occupied};
        ++size_;
        return true;
    }
    throw std::overflow_error("IntHashMap is full");
}

// Mark as Deleted, NOT Empty. An Empty slot would stop later lookups early,
// making keys further along the same cluster (e.g. 17 and 24 after erasing 10)
// unreachable.
bool IntHashMap::erase(int key) {
    const std::size_t i = find_index(key);
    if (i == capacity()) {
        return false;
    }
    slots_[i].state = SlotState::Deleted;
    --size_;
    return true;
}

// Number of slots inspected until the key is found, an Empty slot proves
// absence, or every slot has been inspected.
std::size_t IntHashMap::probe_count(int key) const {
    assert(key >= 0);
    const std::size_t start = bucket(key);
    std::size_t probes = 0;
    for (std::size_t step = 0; step < capacity(); ++step) {
        const std::size_t i = (start + step) % capacity();
        const HashSlot& slot = slots_[i];
        ++probes;
        if (slot.state == SlotState::Empty) {
            return probes;
        }
        if (slot.state == SlotState::Occupied && slot.key == key) {
            return probes;
        }
    }
    return probes;  // == capacity()
}

// Checks: Occupied count == size_, keys non-negative, and every occupied key
// is found by the normal probe rule at exactly the slot where it is stored
// (this also rules out duplicate keys).
bool IntHashMap::check_invariant() const {
    std::size_t occupied = 0;
    for (std::size_t i = 0; i < capacity(); ++i) {
        const HashSlot& slot = slots_[i];
        if (slot.state != SlotState::Occupied) {
            continue;
        }
        ++occupied;
        if (slot.key < 0) {
            return false;
        }
        if (find_index(slot.key) != i) {
            return false;
        }
    }
    return occupied == size_;
}
