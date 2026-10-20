# Week 2 Reflection

Answer briefly but precisely.

## 1. Pointer copying

Why does copying a pointer value not copy the pointed-to object?

A pointer only stores an address. `int* q = p;` copies that address into q, so both pointers now refer to the same object. No new object is created. In `pointer_trace.cpp`, changing `*q` changed `a`, because q pointed to `a`, not to a copy of it.

## 2. Reachability versus lifetime

Explain how an object can still be alive but no longer reachable by traversing from a particular head pointer.

Lifetime depends on how the object was created (for example, a local variable lives until the end of its scope). Reachability only depends on the links. In `node_chain.cpp`, after `a.next = &c;`, node `b` is skipped when traversing from `a`, but `b` is still a live local variable in `main`. Changing a pointer changes the path, not whether the object exists.

## 3. Dangling pointers

When a pointer becomes dangling, what changed: the pointer's stored numeric value, the target object's lifetime, or necessarily both?

Only the target object's lifetime ended. The pointer still stores the same address. That is why a dangling pointer looks valid and why a `!= nullptr` check cannot detect it.

## 4. Ownership responsibility

Why can two pointers to one dynamically allocated object not both independently `delete` it?

One `new` creates one object, which must be released by exactly one `delete`. Two pointers holding the same address do not mean two allocations. After the first `delete`, the second pointer is dangling, and deleting it again is a double free (undefined behaviour, which ASan reported in `double_delete.cpp`). Only one pointer should be the owner; the others are non-owning observers.

## 5. `nullptr`

Why does assigning `nullptr` to a raw pointer not release dynamically allocated storage?

Assigning `nullptr` only changes the value stored in the pointer variable. It does not touch the heap object. Only `delete` releases it. If the only pointer is set to `nullptr` before `delete`, the address is lost and the memory leaks. The correct order is `delete p;` and then `p = nullptr;`.

## 6. Linked traversal complexity

Why is traversal of `n` linked nodes Θ(n) even though following one `next` pointer is Θ(1)?

Each step is Θ(1), but to reach `nullptr` the loop must take one step per node, so there are n steps: n · Θ(1) = Θ(n) time. The auxiliary space is Θ(1), because the traversal only uses one `current` pointer no matter how long the chain is. The nodes themselves occupy Θ(n) memory, but that memory belongs to the structure, not to the traversal algorithm.

## 7. Invariants

State two invariants that should hold for the final chain `10 -> 20 -> 25 -> 30 -> null`.

1. Traversal from the head `a` visits every intended node exactly once, in the order 10, 20, 25, 30 (no cycles, no skipped nodes).
2. The last node (30) has `next == nullptr`, and every non-null `next` points to a node that is still alive.

(Also: insertion only changed links; no node's `value` was copied or modified.)
