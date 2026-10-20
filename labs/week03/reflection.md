# Week 3 Reflection — Dynamic Arrays

Complete this file briefly but precisely.

## 1. Size, capacity, and storage

1. Can `size()` be smaller than `capacity()`? Explain.

   Yes. `size()` is how many elements are actually stored; `capacity()` is how many fit in the current allocation. After growth there are unused slots, e.g. `vector_growth` showed size 5 with capacity 8.

2. Why must `size()` never exceed `capacity()`?

   The elements live inside the allocated array. If size were larger than capacity, the "last" elements would be outside the allocation, so reading or writing them would be out-of-bounds memory access (undefined behaviour).

3. Does every `push_back` allocate? What did `vector_growth` show?

   No. Allocation only happened when the vector was full. Capacity went 1, 2, 4, 8, 16, 32, and `data()` only changed on those pushes (sizes 1, 2, 3, 5, 9, 17). All other pushes reused the existing storage. (At size 3 the new address was the same as the size-1 block, because the allocator reused freed memory — it was still a new allocation.)

4. What does `data()` identify?

   The address of the first element of the vector's current underlying array, i.e. which block of storage the vector is using right now.

## 2. Reallocation and pointer validity

Why can a pointer to an element become invalid after a capacity-changing `push_back`? Explain using **storage lifetime**, not only address changes.

When `push_back` needs more capacity, the vector allocates a new, larger array, copies the elements into it, and then releases the old array. Releasing it ends the lifetime of the old storage and the `int` objects in it. A pointer into the old array (like `old_data` in `reallocation_demo`) still holds the same address, but there is no longer a live object at that address, so it is dangling. Dereferencing it is undefined behaviour, even if the memory happens to still contain the old values or gets reused at the same address.

## 3. Invariants

State the two representation invariants used by `IntVector`. Why is checking an invariant after every mutating operation useful?

1. `0 <= size_ <= capacity_`
2. `capacity_ == 0` if and only if `data_ == nullptr`

Checking them at the end of every operation that changes the object (copy constructor, `grow`, `push_back`) catches a bug at the exact operation that broke the state, instead of much later as a crash or wrong value somewhere else. It also documents what every public operation is allowed to assume and must preserve.

## 4. Complexity

Fill in the table.

| Operation | Complexity | Why? |
|---|---|---|
| `at(i)` | O(1) | Direct address computation `data_ + i`; no search. |
| `push_back` without growth | O(1) | Write one element into a free slot and increment `size_`. |
| `push_back` that reallocates | O(n) | Allocates a new array and copies all n existing elements. |
| `push_back` amortized over many appends | O(1) amortized | Expensive copies happen rarely and their total cost is bounded (see below). |
| copy construction | O(n) | Allocates new storage and copies all n elements. |

Why does doubling capacity give amortized constant-time append even though some individual appends are linear?

Reallocations happen only at sizes 1, 2, 4, 8, ..., so for n appends the total number of element copies is at most 1 + 2 + 4 + ... + n/2 < n. Adding the n ordinary writes, n appends cost less than 2n operations in total, which is O(n) overall, i.e. O(1) per append on average. Growing by a constant amount instead (e.g. +1) would copy 1 + 2 + 3 + ... ≈ n²/2 elements, i.e. O(n) per append.

## 5. Deep copy

What would go wrong if copying `IntVector` only copied `data_`, `size_`, and `capacity_` member-by-member? Name at least two correctness/ownership problems.

1. **Double free:** both objects would hold the same `data_` and both destructors would `delete[]` it.
2. **Shared mutation:** `copy.at(0) = 999` would also change the original, because they share one array (the public test for independent storage would fail).
3. **Dangling pointer / stale size:** if one object grows, it frees the shared array, leaving the other object pointing to released storage, and the two `size_` values no longer describe the same data.

## 6. STL comparison

Give one reason to use `std::vector<int>` in production code instead of this teaching implementation, and one reason implementing `IntVector` is still useful in a data-structures course.

Production: `std::vector` is thoroughly tested, exception-safe, has the full Rule of Five (copy/move assignment, move construction), iterators, `reserve`, `insert`/`erase` and works with standard algorithms, so it is safer and more complete than writing my own.

Course: implementing `IntVector` shows what `std::vector` does internally — size vs capacity, reallocation, why pointers get invalidated, ownership and deep copy, and why doubling gives amortized O(1) append. That makes it easier to reason about performance and bugs when using the real `std::vector`.
