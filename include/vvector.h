#ifndef VVECTOR_H_
#define VVECTOR_H_

#include <algorithm>

#include "include/gcontainer.h"

using vid_t = unsigned;

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

using VNode = Handle<vid_t>;

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

using VRange = Range<vid_t>;

template <typename T, typename U>
class rvector : public garray<T> {
 public:
  rvector() : garray<T>() {}
  explicit rvector(const Range<U>& range)
      : garray<T>(range.size()), range_(range) {}
  rvector(const Range<U>& range, const T& value)
      : garray<T>(range.size(), value), range_(range) {}

  ~rvector() {}

  void Init(const Range<U>& range, const T& value) {
    garray<T>::clear();
    garray<T>::resize(range.size(), value);
    range_ = range;
  }

  void SetValue(Range<U>& range, const T& value) {
    std::fill_n(&garray<T>::start_[range.begin().GetValue() -
                                   range_.begin().GetValue()],
                range.size(), value);
  }

  void SetValue(const T& value) {
    std::fill(garray<T>::start_, garray<T>::end_, value);
  }

  inline T& operator[](const Handle<U>& loc) {
    return garray<T>::start_[loc.GetValue() - range_.begin().GetValue()];
  }

 private:
  Range<U> range_;
};

using VVector<T> = rvector<T, vid_t>;

#include <functional>

namespace std {

template <>
struct hash<Handle<vid_t>> {
  inline size_t operator()(const Handle<vid_t>& obj) const {
    return hash<vid_t>()(obj.GetValue());
  }
};

}  // namespace std

#endif  // VVECTOR_H_