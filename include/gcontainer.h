#ifndef GCONTAINER_H_
#define GCONTAINER_H_

#include "include/gtype_traits.h"
#include "include/uninitialized.h"

#ifdef NO_NUMA
#define USE_REALLOC
#define GMALLOC(size) malloc((size))
#define GFREE(buffer, size) free((buffer))
#define GREALLOC(buffer, old_size, new_size) realloc((buffer), (new_size))
#else
#include <numa.h>
// #define USE_REALLOC
#define GMALLOC(size) numa_alloc_local((size))
#define GFREE(buffer, size) numa_free((buffer), (size))
#define GREALLOC(buffer, old_size, new_size) \
  numa_realloc((buffer), (old_size), (new_size))
#endif

#define BLOCK_SIZE 128
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ROUND_UP(bytes) (((bytes) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

template <typename T>
class gvector {
 public:
  gvector() : start_(NULL), finish_(NULL), end_of_storage_(NULL) {}

  explicit gvector(size_t n) {
    start_ = reinterpret_cast<T *>(GMALLOC(n * sizeof(T)));
    finish_ = uninitialized_gfill_n(start_, n, T());
    end_of_storage_ = start_ + n;
  }

  gvector(size_t n, const T &value) {
    start_ = reinterpret_cast<T *>(GMALLOC(n * sizeof(T)));
    finish_ = uninitialized_gfill_n(start_, n, value);
    end_of_storage_ = start_ + n;
  }

  ~gvector() {
    if (start_ != NULL) {
      _Destroy(start_, finish_);
      GFREE(start_, end_of_storage_ - start_);
    }
  }

  size_t size() const { return finish_ - start_; }
  size_t capacity() const { return end_of_storage_ - start_; }

  void resize(size_t n, const T &val) {
    size_t _size = finish_ - start_;
    if (n < _size) {
      _Destroy(start_ + n, finish_);
      finish_ = start_ + n;
    } else {
      size_t _capacity = end_of_storage_ - start_;
      if (n < _capacity) {
        finish_ = uninitialized_gfill_n(finish_, n - _size, val);
      } else {
        size_t new_capacity = MAX(_capacity << 1, ROUND_UP(n));
        T *new_start = reinterpret_cast<T *>(GMALLOC(new_capacity * sizeof(T)));
        T *new_finish = uninitialized_gcopy(start_, finish_, new_start);
        new_finish = uninitialized_gfill_n(new_finish, n - _size, val);
        _Destroy(start_, finish_);
        GFREE(start_, _capacity);
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = new_start + new_capacity;
      }
    }
  }
  void resize(size_t n) { resize(n, T()); }

  void swap(gvector &rhs) {
    std::swap(start_, rhs.start_);
    std::swap(finish_, rhs.finish_);
    std::swap(end_of_storage_, rhs.end_of_storage_);
  }

  void push_back(const T &x) { resize(size() + 1, x); }

  T *begin() { return start_; }
  const T *begin() const { return (const T *)start_; }
  T *end() { return finish_; }
  const T *end() const { return (const T *)finish_; }

  T &operator[](size_t n) { return *(start_ + n); }
  const T &operator[](size_t n) const { return *(start_ + n); }

  T *start_;
  T *finish_;
  T *end_of_storage_;
};

#endif
