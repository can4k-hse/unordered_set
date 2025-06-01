# 🗃️ UnorderedSet

[![License: CC0 1.0](https://img.shields.io/badge/License-CC0_1.0-lightgrey.svg)](https://creativecommons.org/publicdomain/zero/1.0/)

`UnorderedSet<KeyT>` is a generic container based on a hash table with separate chaining, similar to `std::unordered_set` from the C++ Standard Library.

This container supports efficient **amortized O(1)** insert, erase, and lookup operations, automatically grows the number of buckets when the load factor exceeds a threshold, and maintains stable performance even with a large number of elements.

---

## ✨ Features

- 📦 **Hash table with separate chaining**: each element is placed into a bucket based on the result of a hash function.
- 🔁 **Dynamic resizing**: if the load factor reaches 1.0, the container doubles the number of buckets and redistributes the elements.
- ⚡ **Fast operations**:
  - Insertion (`Insert`)
  - Erasure (`Erase`)
  - Lookup (`Find`)
- 🔄 **Manual control over the hash table**:
  - `Rehash(new_bucket_count)` — changes the number of buckets and rehashes all elements
  - `Reserve(new_bucket_count)` — reserves space for buckets if the requested count is larger than current
- 🧾 **State information**:
  - `Size()` — number of elements
  - `Empty()` — checks if the set is empty
  - `Clear()` — clears all elements
  - `BucketCount()` — current number of buckets
  - `BucketSize(id)` — number of elements in the specified bucket
  - `Bucket(key)` — index of the bucket where a key is stored
  - `LoadFactor()` — ratio of elements to buckets

---

## 🛠️ Constructors

- `UnorderedSet()` — creates an empty table
- `UnorderedSet(size_t count)` — creates a table with `count` empty buckets
- `UnorderedSet(Iter first, Iter last)` — creates a table and fills it with elements from the range `[first, last)`

Also supports:
- Copy and move constructors
- Copy and move assignment operators

---

## 🧠 Implementation details

- Uses `std::hash<KeyT>` to compute hashes
- Internally based on `std::vector<std::list<KeyT>>`
- Efficient rehashing with pointer reuse via `std::list::splice`
- Safe copy/move semantics

---

## 📈 When to use

- You need fast access by key without preserving order
- You want to prevent duplicate keys
- You work with large datasets where fast insert, lookup, and delete operations are crucial

---

## 📂 Project Structure
- `LICENSE` — Public domain license (CC0)
- `README.md` — Project documentation
- `unordered_set.h` — Main implementation of `UnorderedSet`
- `.clang-format` — Code formatting settings
- `.clang-tidy` — Linting configuration
- `tests/unordered_set_public_test.cpp` — test cases
---

## 🧪 Tests

This project uses [Catch2](https://github.com/catchorg/Catch2) as the testing framework.  
To use tests:

1. **Download `catch.hpp`:**  
   Download the single-header version of Catch2 (`catch.hpp`) from the official [Catch2 releases](https://github.com/catchorg/Catch2/releases) or search for it online.  

2. **Place** `catch.hpp` in your root directory.

---

<div style="background-color: rgba(255, 165, 0, 0.1); border-left: 4px solid orange; padding: 1em; font-size: 0.95em; margin-top: 12px">

🔶 <strong>Note:</strong> this <code>README.md</code> file and comments in the source code were partially or fully generated with the help of artificial intelligence (AI).
</div>
