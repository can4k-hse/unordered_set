// ---------------------------------------------------------------------------------
// Most comments in this file were generated with the help of artificial intelligence (AI).
// The code itself was written by the author.
// ---------------------------------------------------------------------------------
// Filename: unordered_set.hpp
// Author: Jakovlev Alexandr
// Direction: HSE FCS SE
// Discipline: Basics of C++ programming
// Group: 244-2
// CreatedAt: 2025-06-01
// ---------------------------------------------------------------------------------

#ifndef UNORDERED_SET_HPP
#define UNORDERED_SET_HPP

#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

/// @brief UnorderedSet is a hash-based set container that stores unique keys with fast insertion, removal, and lookup.
/// Internally uses separate chaining with std::vector<std::list<KeyT>> for collision resolution.
/// Does not preserve element order. Duplicate keys are not allowed.
/// @tparam KeyT Type of the elements (keys) stored in the set.
template <class KeyT>
class UnorderedSet {
 public:
  /// ------ ALIASES ------ ///
  using Type = KeyT;
  using CType = const KeyT;
  using Ptr = KeyT*;
  using CPtr = const KeyT*;
  using Ref = KeyT&;
  using CRef = const KeyT&;
  using RRef = KeyT&&;
  using SzType = size_t;

  /// @brief The maximum allowed load factor before triggering a rehash.
  /// When the number of elements exceeds kUltimateLoadFactor * bucket count, the set should be rehashed.
  static constexpr long double kUltimateLoadFactor = 1;

  /// @brief The coefficient by which the number of buckets increases during a rehash.
  static constexpr SzType kRehashCoff = 2;

 private:
  /// ------ FIELDS ------ ///

  /// @brief Buckets for separate chaining: each bucket is a list of elements with the same hash modulo bucket count.
  std::vector<std::list<Type>> buckets_ = {};

  /// @brief Number of elements in all buckets.
  SzType size_ = 0;

  /// @brief Hash functor used to compute hash values for keys.
  std::hash<Type> hasher_ = std::hash<Type>();

 public:
  /// ------ CONSTRUCTORS ------ ///

  UnorderedSet() = default;

  /// @brief Constructs an UnorderedSet with a specified number of empty buckets.
  /// @param count Number of buckets to allocate.
  explicit UnorderedSet(SzType count) {
    buckets_ = std::vector<std::list<Type>>(count);
    size_ = 0;
  }

  /// @brief Constructs an UnorderedSet from a range of elements.
  /// @tparam Iterator Forward iterator type.
  /// @param begin Iterator to the first element in the range.
  /// @param end Iterator to one past the last element in the range.
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  UnorderedSet(Iterator begin, Iterator end) {
    for (auto it = begin; it != end; it++) {
      Insert(*it);
    }
  }

  UnorderedSet(const UnorderedSet& other) {
    *this = other;
  }

  UnorderedSet(UnorderedSet&& other) {
    *this = std::move(other);
  }

  /// ------ OPERATORS ------ ///

  UnorderedSet& operator=(const UnorderedSet& other) = default;

  /// @brief Move copy. Transfers resources from another UnorderedSet.
  /// @param other The UnorderedSet to move from.
  UnorderedSet& operator=(UnorderedSet&& other) {
    buckets_ = std::move(other.buckets_);
    size_ = std::move(other.size_);
    hasher_ = std::move(other.hasher_);
    other.size_ = 0;  // do not forget
    return *this;
  }

 private:
  ///------- ENCAPSULATED METHODS-------///

  /// @brief Returns a reference to the bucket (list) corresponding to the given element.
  /// @param element The key whose bucket is to be found.
  /// @return Reference to the list representing the bucket for the element.
  std::list<Type>& GetElementBucket(CType element) {
    auto bucket_index = Bucket(element);
    return buckets_[bucket_index];
  }

  /// @brief Returns a const reference to the bucket (list) corresponding to the given element.
  /// @param element The key whose bucket is to be found.
  /// @return Const reference to the list representing the bucket for the element.
  const std::list<Type>& GetElementBucket(CType element) const {
    auto bucket_index = Bucket(element);
    return buckets_[bucket_index];
  }

 public:
  /// ------ METHODS ------ ///

  /// @brief Returns the number of elements in the set.
  inline SzType Size() const {
    return size_;
  }

  /// @brief Checks if the set is empty.
  /// @return true if the set contains no elements, false otherwise.
  inline bool Empty() const {
    return size_ == 0;
  }

  /// @brief Removes all elements from the set.
  inline void Clear() {
    *this = UnorderedSet();
  }

  /// @brief Checks if the set contains the specified element.
  /// @param element The element to search for.
  /// @return true if the element is found, false otherwise.
  bool Find(CRef element) const {
    if (Empty()) {
      return false;
    }

    const auto& element_bucket = GetElementBucket(element);
    auto it = std::find(element_bucket.begin(), element_bucket.end(), element);
    return it != element_bucket.end();
  }

  /// @brief Inserts a new element into the set if it is not already present.
  /// @tparam T Type of the element (deduced automatically).
  /// @param element The element to insert (can be lvalue or rvalue).
  template <class T = Type>
  void Insert(T&& element) {
    if (Find(element)) {
      return;
    }

    if (static_cast<long double>(Size() + 1) > BucketCount() * kUltimateLoadFactor) {  // load factor overflow
      Rehash(std::max(static_cast<SzType>(1), BucketCount() * kRehashCoff));
    }

    GetElementBucket(element).emplace_back(std::forward<T>(element));
    size_++;
  }

  /// @brief Removes the specified element from the set if it exists.
  /// @param element The element to remove.
  void Erase(CRef element) {
    if (Empty()) {
      return;
    }

    auto& element_bucket = GetElementBucket(element);
    auto it = std::find(element_bucket.begin(), element_bucket.end(), element);

    if (it != element_bucket.end()) {  // element was founded
      element_bucket.erase(it);
    }

    size_--;
  }

  /// @brief Rehashes the set to use a new number of buckets and redistributes all elements.
  /// @param new_bucket_count The new number of buckets (must be greater than current and geq than the (number of
  /// elements) / LoadFactor).
  /// @note If the requested bucket equals to the current, or less than the number of elements, nothing happens.
  /// @note All elements are reinserted into the new bucket structure.
  void Rehash(SzType new_bucket_count) {
    if (new_bucket_count == BucketCount()) {
      return;  // nothing to do
    }

    auto sz_ld = static_cast<long double>(Size());
    auto buckets_count_ld = static_cast<long double>(new_bucket_count);

    // checks if rehashing is possible
    if (sz_ld > buckets_count_ld * kUltimateLoadFactor) {
      return;
    }

    // copies elements
    auto tmp = UnorderedSet(new_bucket_count);

    for (const auto& bucket : buckets_) {
      for (CRef element : bucket) {
        tmp.Insert(element);
      }
    }

    *this = tmp;
  }

  /// @brief Ensures that the set has at least the specified number of buckets.
  /// @param new_bucket_count The minimum number of buckets to reserve.
  void Reserve(SzType new_bucket_count) {
    if (new_bucket_count <= BucketCount()) {
      return;
    }

    Rehash(new_bucket_count);
  }

  /// @brief Returns the total number of buckets in the set.
  /// @return Number of buckets.
  inline SzType BucketCount() const {
    return buckets_.size();
  }

  /// @brief Returns the number of elements in the specified bucket.
  /// @param id Index of the bucket.
  /// @return Number of elements in the bucket, or 0 if id is out of range.
  inline SzType BucketSize(SzType id) const {
    if (id >= buckets_.size()) {  //
      return 0;
    }
    return buckets_[id].size();
  }

  /// @brief Returns the index of the bucket for the given element.
  /// @param element The element whose bucket index is requested.
  /// @return Index of the bucket where the element would be placed.
  inline SzType Bucket(CRef element) const {
    return hasher_(element) % (buckets_.size());
  }

  /// @brief Returns the load factor of the set (average number of elements per bucket).
  /// @return The load factor as a floating-point value.
  double LoadFactor() const {
    if (Size() == 0) {
      return 0;  // edge case
    }
    return 1.0 * Size() / BucketCount();
  }
};

#endif  // UNORDERED_SET_HPP