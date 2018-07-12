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

  void Swap(Handle& rhs) { std::swap(value_, rhs.value_); }

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

  void Swap(Range& rhs) {
    begin_.Swap(rhs.begin_);
    end_.Swap(rhs.end_);
    std::swap(size_, rhs.size_);
  }

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

  void SetValue(Range<U>& range, const T& value) {
    std::fill_n(&garray<T>::start_[range.begin().GetValue() - range_.begin().GetValue()],
                range.size(), value);
  }

  void SetValue(const T& value) { std::fill(garray<T>::start_, garray<T>::end_, value); }

  inline T& operator[](const Handle<U>& loc) {
    return garray<T>::operator[](loc.GetValue() - range_.begin().GetValue());
  }
  inline const T& operator[](const Handle<U>& loc) const {
    return garray<T>::operator[](loc.GetValue() - range_.begin().GetValue());
  }

  void Swap(rvector& rhs) {
    garray<T>::Swap((garray<T>&)rhs);
    range_.Swap(rhs.range_);
  }

 private:
  Range<U> range_;
};

template <typename T, typename U>
class nrvector : public garray<T> {
 public:
  nrvector() : garray<T>(), new_begin_(NULL) {}
  explicit nrvector(const Range<U>& range)
      : garray<T>(range.size()), range_(range) {
    new_begin_ = garray<T>::start_ - range_.begin().GetValue();
  }
  nrvector(const Range<U>& range, const T& value)
      : garray<T>(range.size(), value), range_(range) {
    new_begin_ = garray<T>::start_ - range_.begin().GetValue();
  }

  ~nrvector() {}

  void SetValue(Range<U>& range, const T& value) {
    std::fill_n(&garray<T>::start_[range.begin().GetValue() - range_.begin().GetValue()],
                range.size(), value);
  }

  void SetValue(const T& value) { std::fill(garray<T>::start_, garray<T>::end_, value); }

  inline T& operator[](const Handle<U>& loc) {
    return new_begin_[loc.GetValue()];
    // return garray<T>::operator[](loc.GetValue() - range_.begin().GetValue());
  }
  inline const T& operator[](const Handle<U>& loc) const {
    return new_begin_[loc.GetValue()];
    // return garray<T>::operator[](loc.GetValue() - range_.begin().GetValue());
  }

  void Swap(nrvector& rhs) {
    garray<T>::Swap((garray<T>&)rhs);
    range_.Swap(rhs.range_);
    std::swap(new_begin_, rhs.new_begin_);
  }

 private:
  Range<U> range_;
  T* new_begin_;
};

template <typename T>
using VVector = rvector<T, vid_t>;

template <typename T>
using NVVector = nrvector<T, vid_t>;

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
