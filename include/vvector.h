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
class rvector : public GArray<T> {
 public:
  rvector() : GArray<T>(), fake_start_(NULL) {}
  explicit rvector(const Range<U>& range)
      : GArray<T>(range.size()), range_(range) {
    fake_start_ = GArray<T>::start_ - range_.begin().GetValue();
  }
  rvector(const Range<U>& range, const T& value)
      : GArray<T>(range.size(), value), range_(range) {
    fake_start_ = GArray<T>::start_ - range_.begin().GetValue();
  }

  ~rvector() {}

  void Reset(const Range<U>& range) {
    GArray<T>::Resize(range.size());
    fake_start_ = GArray<T>::start_ - range_.begin().GetValue();
  }

  void Reset(const Range<U>& range, const T& value) {
    GArray<T>::Resize(range.size(), value);
    fake_start_ = GArray<T>::start_ - range_.begin().GetValue();
  }

  void SetValue(Range<U>& range, const T& value) {
    std::fill_n(&GArray<T>::start_[range.begin().GetValue() -
                                   range_.begin().GetValue()],
                range.size(), value);
  }

  void SetValue(const T& value) {
    std::fill(GArray<T>::start_, GArray<T>::end_, value);
  }

  inline T& operator[](const Handle<U>& loc) {
    return fake_start_[loc.GetValue()];
  }
  inline const T& operator[](const Handle<U>& loc) const {
    return fake_start_[loc.GetValue()];
  }

  void Swap(rvector& rhs) {
    GArray<T>::Swap((GArray<T>&)rhs);
    range_.Swap(rhs.range_);
    std::swap(fake_start_, rhs.fake_start_);
  }

 private:
  void Resize() {}
  void Clear() {}

  Range<U> range_;
  T* fake_start_;
};

template <typename T>
using VVector = rvector<T, vid_t>;

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
