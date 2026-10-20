# Week 4 Reflection

Answer concisely after your implementation passes the public tests and sanitizer run.

1. **Why can logical adjacency differ from physical address order in a linked list?**

   Each node is allocated separately with `new`, so nodes can be anywhere in memory. Order is defined only by the `next` pointers, not by where the nodes are stored. Inserting 30 after 20 makes 20 → 30 logically adjacent, even though node 30 was allocated later at an unrelated address.

2. **Why is numeric indexed access Θ(n) in a simple singly linked list?**

   There is no address formula like `data_ + i`. To reach position i you must start at `head_` and follow `next` i times, so the worst case (the last element) takes n − 1 steps.

3. **Under what precise precondition is insertion by pointer rewiring Θ(1)?**

   When you already have a pointer to the node you want to insert after (or you insert at `head_`, or at `tail_` because it is cached). Then only a constant number of pointers change: the new node's `next`, the predecessor's `next`, and possibly `tail_`.

4. **Why is `insert_after_first(target, value)` still Θ(n) in the worst case?**

   The operation must first find the target by traversing from `head_`. If the target is the last node or absent, that navigation visits all n nodes. The rewiring itself is Θ(1), but navigation dominates: Θ(n) + Θ(1) = Θ(n).

5. **What invariant responsibility is added by caching `tail_`?**

   Every operation that can change the last node must keep `tail_` correct: it must point to the final reachable node, with `tail_->next == nullptr`, and be `nullptr` exactly when the list is empty. That means updating it in `push_back`, `push_front` on an empty list, `pop_front` to empty, inserting after the tail, erasing the tail, and `clear`. This is the price of Θ(1) `push_back` and `back()`.

6. **Why must a removed node's successor be obtained before deleting the node?**

   `delete` ends the node's lifetime. Reading `removed->next` afterwards is a use-after-free (undefined behaviour), and the rest of the chain would be lost or reached through garbage. So I save `removed->next` (and `current->next` in `clear()`) first, then delete.

7. **Give one structural bug that a memory sanitizer may not directly identify as a linked-list invariant violation.**

   Forgetting to update `tail_` after erasing the last node. If the node is deleted, ASan catches it only when someone later uses the stale `tail_`. If instead the erased node is unlinked but never deleted (a leak plus stale tail), or `size_` is off by one, or `tail_` points to a live but non-final node, every access is to valid memory, so no memory error is reported, but the structure is wrong. `check_invariant()` catches these.

8. **Why can dynamic-array traversal outperform linked-list traversal even though both are Θ(n)?**

   Array elements are contiguous, so the CPU cache loads neighbouring elements together and the hardware can prefetch ahead. Linked nodes are scattered, so each step is pointer chasing: the next address is only known after loading the current node, often causing a cache miss. Each node also stores an extra pointer, so more memory is read per element. Same Θ(n), much larger constant factor.

9. **Give one workload favoring the Week 4 representation and one favoring Week 3's dynamic array.**

   Linked list: a queue of tasks with many insertions and removals at the front (`push_front` / `pop_front` are Θ(1) with no shifting), or many inserts/erases right after nodes you already hold. Dynamic array: reading and processing a large dataset, with frequent random access by index (Θ(1) `at(i)`) and appends at the end.

10. **How do these representation choices prepare you to implement stacks and queues next week?**

    A stack only needs push/pop at one end: `push_front`/`pop_front` on the linked list, or `push_back`/pop at the end of the dynamic array, all Θ(1). A queue needs insertion at one end and removal at the other: the linked list with cached `tail_` gives `push_back` + `pop_front` in Θ(1), while a plain dynamic array would need Θ(n) shifting on front removal (so it needs a circular buffer). Knowing which end of each representation is cheap tells me how to build both.
