# Week 6 Reflection — Linear-Probing Hash Map

Answer briefly after your implementation works.

1. **Under suitable hashing and controlled load, why are lookup, insertion, and deletion described as **expected Θ(1)** rather than worst-case Θ(1)?**

   With keys spread evenly and a load factor kept below a constant (e.g. ≤ 0.7), the average probe sequence has constant length, so the *expected* cost is Θ(1). But nothing prevents bad cases: if many keys collide (or the table is nearly full), one operation may have to inspect up to all n / capacity slots, which is Θ(n). So Θ(1) describes the average under assumptions, not a guarantee for every operation.

2. **Give a set of integer keys that all collide when the table capacity is 7. What happens to their probe lengths?**

   10, 17, 24 (all ≡ 3 mod 7). They form one cluster at slots 3, 4, 5, so their probe counts are 1, 2 and 3. Another set is 6, 13, 20, which prefer slot 6 and wrap around to slots 0 and 1 (probe counts 1, 2, 3). In general k colliding keys need 1, 2, …, k probes — linear growth, so a fully colliding table degrades to Θ(n) lookup.

3. **Why must lookup continue through a Deleted slot but may stop at an Empty slot?**

   An Empty slot has never held an entry in this probe run, so the key could not have been placed beyond it — insertion would have used it. A Deleted slot once held a key, so other keys may have been placed after it while it was occupied. Example: after erasing 10, slot 3 is a tombstone; stopping there would make 17 (slot 4) and 24 (slot 5) unreachable.

4. **Why must insert_or_assign continue probing after finding a tombstone before deciding to insert there?**

   The key might already exist later in the same probe sequence (it was inserted when the tombstone's slot was still occupied). Example: insert 10, 17, 24, erase 10, then `insert_or_assign(24, …)`. Inserting at slot 3 would create a second 24. So I remember the first tombstone, keep probing until I find the key (update it) or reach an Empty slot / complete a full cycle, and only then insert at the remembered tombstone.

5. **Why does increasing the load factor usually increase successful and unsuccessful probe lengths?**

   More occupied slots means a higher chance that the preferred bucket is taken, and occupied slots merge into longer clusters (primary clustering). An unsuccessful search must walk to the end of the cluster to find an Empty slot, and new keys get placed at cluster ends, making later searches longer too. Near load factor 1, probe lengths grow very quickly.

6. **Why can changing the table capacity require reinserting or rehashing every live key?**

   Each key's position depends on `key % capacity`. With a new capacity the preferred buckets change (e.g. 10 % 7 = 3 but 10 % 11 = 10), and the probe sequences change too, so keys left in their old slots would no longer be found. Every live key must be inserted again into the new table (tombstones are dropped), which is Θ(n).

7. **How is **expected Θ(1)** hash-table lookup different from **amortized Θ(1)** dynamic-array append?**

   Amortized Θ(1) is a guarantee about the total cost of any sequence of operations: n appends always cost O(n) in total, even though a few individual appends are Θ(n). Expected Θ(1) is an average over the key distribution / hash behaviour: it holds when keys hash well, but a bad set of keys can make *every* lookup Θ(n), and no sequence-level guarantee prevents that.

8. **Give one tradeoff between separate chaining and open addressing.**

   Open addressing stores everything in one contiguous array (good cache locality, no per-node allocation) but needs tombstones, suffers from clustering, and degrades badly as the load factor approaches 1, so it cannot exceed capacity. Separate chaining handles high load factors and deletion more simply (just remove from a bucket list) but uses extra memory for list nodes/pointers and has worse locality due to pointer chasing.

## Comparison with `std::unordered_map<int, int>`

The same dictionary-level operations behave the same: `insert_or_assign` returns "inserted" for a new key and "updated" for an existing key without changing `size()`, `find`/`contains` locate keys, and `erase` removes a key and decreases `size()`. The internal representation is different (the standard library typically uses buckets of nodes, i.e. chaining, and rehashes automatically), and `std::unordered_map` grows itself instead of throwing when full.
