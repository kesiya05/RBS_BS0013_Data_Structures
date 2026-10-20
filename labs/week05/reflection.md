# Week 5 Reflection — Circular Queue

Answer briefly after your implementation works.

1. **Why is `enqueue` Θ(1) **worst case** in this fixed-capacity queue?**

   It computes one index `(front_ + size_) % capacity()`, writes one value and increments `size_`. The capacity is fixed, so there is never a reallocation or copying of elements, and every call does the same constant amount of work. If the queue is full it just throws.

2. **Why is `dequeue` Θ(1) worst case?**

   It only moves `front_` one step forward with `(front_ + 1) % capacity()` and decrements `size_`. No remaining element is shifted, unlike a plain array queue where removing from index 0 shifts n − 1 elements.

3. **Why would a dynamically growing circular queue normally describe enqueue as **amortized Θ(1)** instead?**

   When it is full it must allocate a bigger array and copy all n elements into it (unwrapping them in logical order), so that single enqueue is Θ(n). With doubling, these copies are rare, and over many enqueues the total cost is O(n), so the average per enqueue is Θ(1) — but not every individual call.

4. **In the expression `(front_ + i) % capacity()`, what does `i` mean: a physical index or a logical position?**

   A logical position: `i = 0` is the front (oldest) element, `i = size_ - 1` is the back. The expression converts it into the physical array index. Example: capacity 5, `front_ = 3`, `size_ = 4` → logical 0, 1, 2, 3 are at physical 3, 4, 0, 1 (wrapping past the end).

5. **Why can the physical array still contain an old integer after `dequeue()` without that value remaining part of the logical queue?**

   `dequeue()` does not erase anything; it only moves `front_` and reduces `size_`. The queue's contents are defined by `front_` and `size_`: only positions `(front_ + i) % capacity()` for `0 <= i < size_` belong to the queue. The old value sits in an unused slot and will be overwritten by a later `enqueue`.

6. **Give one practical advantage of a circular array over a linked queue.**

   Elements are stored contiguously in one allocation, so there is no `new`/`delete` per element, no extra `next` pointer per element, and much better cache locality.

7. **Give one practical limitation of this fixed-capacity circular queue.**

   The capacity is fixed at construction. When it is full, `enqueue` fails (throws) instead of growing, so you must know the maximum size in advance, and a large capacity wastes memory if the queue is mostly empty.

8. **Which of these statements belongs to the **queue ADT**, and which belongs only to our **representation**?**
   - first inserted remaining element is removed first; → **ADT** (FIFO behaviour)
   - storage is a `std::vector<int>`; → **representation**
   - elements may wrap around index 0; → **representation**
   - `front()` returns the next element to be removed. → **ADT**
