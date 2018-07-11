#ifndef RVECTOR_H_
#define RVECTOR_H_

#include <algorithm>

#include "include/gcontainer.h"

template <typename T>
class Handle {
 public:
  Handle() {}
  explicit Handle(T value) : value_(value) {}
  Handle(const Handle& rhs) : value_(rhs.value_) {}
  Handle(Handle&& rhs) : value_(rhs.value_) {}

  ~Handle() {}

  inline Handle& operator=(const Handle& rhs) {
    value_ = rhs.value_;
    return *this;
  }

  inline Handle& operator=(Handle&& rhs) {
    value_ = rhs.value_;
    return *this;
  }

  inline Handle& operator=(T value) {
    value_ = value;
    return *this;
  }

  inline Handle& operator++() {
    value_++;
    return *this;
  }

  inline Handle operator++(int) {
    Handle res(value_);
    value_++;
    return res;
  }

  inline bool operator==(const Handle& rhs) const {
    return value_ == rhs.value_;
  }

  inline bool operator!=(const Handle& rhs) const {
    return value_ != rhs.value_;
  }

  inline bool operator<(const Handle& rhs) const { return value_ < rhs.value_; }

  inline Handle& operator*() { return *this; }

  inline T GetValue() const { return value_; }

  inline void SetValue(T value) { value_ = value; }

  friend InArchive& operator<<(InArchive& archive, const Handle& h) {
    archive << h.value_;
    return archive;
  }

  friend OutArchive& operator>>(OutArchive& archive, Handle& h) {
    archive >> h.value_;
    return archive;
  }

  friend class MessageBuffer;

 private:
  T value_;
};

template <typename T>
bool operator<(Handle<T> const& lhs, Handle<T> const& rhs) {
  return lhs.GetValue() < rhs.GetValue();
}

template <typename T>
bool operator==(Handle<T> const& lhs, Handle<T> const& rhs) {
  return lhs.GetValue() == rhs.GetValue();
}

template <typename T>
class Range {
 public:
  Range() {}
  Range(T begin, T end) : begin_(begin), end_(end), size_(end - begin) {}
  Range(const Handle<T>& begin, const Handle<T>& end)
      : begin_(begin), end_(end), size_(end.GetValue() - begin.GetValue()) {}
  Range(const Range& r) : begin_(r.begin_), end_(r.end_), size_(r.size_) {}

  inline const Handle<T>& begin() { return begin_; }

  inline const Handle<T>& end() { return end_; }

  inline size_t size() const { return size_; }

 private:
  Handle<T> begin_, end_;
  size_t size_;
};

template <typename T, typename U>
class rvector : protected gvector<T> {
 public:
  rvector() : gvector<T>() {}
  explicit rvector(const Range<U>& range)
      : gvector<T>(range.size()), range_(range) {}
  rvector(const Range<U>& range, const T& value)
      : gvector<T>(range.size(), value), range_(range) {}

  ~rvector() { ~gvector<T>(); }

  void Init(const Range<U>& range, const T& value) {
    gvector<T>::clear();
    gvector<T>::resize(range.size(), value);
    range_ = range;
  }

  void SetValue(Range<U>& range, const T& value) {
    std::fill_n(&start_[range.begin().GetValue() - range_.begin().GetValue()],
                range.size(), value);
  }

  void SetValue(const T& value) { std::fill(start_, finish_, value); }

  inline T& operator[](const Handle<U>& loc) {
    return start_[loc.GetValue() - range_.begin().GetValue()];
  }

 private:
  Range<U> range_;
};

#include <functional>

namespace std {

template <>
struct hash<Handle<unsigned>> {
  inline size_t operator()(const Handle<unsigned>& obj) const {
    return hash<unsigned>()(obj.GetValue());
  }
};

}  // namespace std

#endif  // RVECTOR_H_
