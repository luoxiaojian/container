#include "gtype_traits.h"
#include "uninitialized.h"

#ifdef NO_NUMA
#define USE_REALLOC
#define GMALLOC(size) malloc((size))
#define GFREE(buffer, size) free((buffer))
#define GREALLOC(buffer, old_size, new_size) realloc((buffer), (new_size))
#else
#include <numa.h>
#define USE_REALLOC
#define GMALLOC(size) numa_alloc_local((size))
#define GFREE(buffer, size) numa_free((buffer), (size))
#define GREALLOC(buffer, old_size, new_size) \
  numa_realloc((buffer), (old_size), (new_size))
#endif

template <typename T>
class __basic_gvector {
 public:
  __basic_gvector()
      : _M_start(NULL), _M_finish(NULL), _M_end_of_storage(NULL) {}

  explicit __basic_gvector(size_t n) {
    _M_start = reinterpret_cast<T *>(GMALLOC(n * sizeof(T)));
    _M_finish = uninitialized_gfill_n(_M_start, n, T());
    _M_end_of_storage = _M_start + n;
  }

  __basic_gvector(size_t n, const T &value) {
    _M_start = reinterpret_cast<T *>(GMALLOC(n * sizeof(T)));
    _M_finish = uninitialized_gfill_n(_M_start, n, value);
    _M_end_of_storage = _M_start + n;
  }

  ~__basic_gvector() {
    if (_M_start != NULL) {
      _Destroy(_M_start, _M_finish);
      GFREE(_M_start, _M_end_of_storage - _M_start);
    }
  }

  size_t size() const { return _M_finish - _M_start; }
  size_t capacity() const { return _M_end_of_storage - _M_start; }

  void __resize(size_t n, const T &val) {}
  void __resize(size_t n, const 

  T *_M_start;
  T *_M_finish;
  T *_M_end_of_storage;
};

template <typename _Tp>
class gvector : protected __basic_gvector<_Tp> {
 public:
  gvector(size_t n, const _Tp &value) : __basic_gvector(n) {
    _M_finish = uninitialized_gfill_n(_M_start, n, value);
  }
  explicit gvector(size_t n) : __basic_gvector(n) {
    _M_finish = uninitialized_gfill_n(_M_start, n, _Tp());
  }

  gvector(const gvector &__x) : __basic_gvector(__x, size()) {
    _M_finish = uninitialized_gcopy(__x.begin(), __x.end(), _M_start);
  }

  ~gvector() { _Destroy(_M_start, _M_finish); }

  T *begin() { return _M_start; }
  T *end() { return _M_finish; }

  size_t size() const { return size_t(_M_finish - _M_start); }
  size_t capacity() const { return size_t(_M_end_of_storage - _M_start); }

  void resize(size_t __new_size, const _Tp &__x) {
    if (__new_size < size()) {
      _Destroy(_M_start + __new_size, _M_finish);
      _M_finish = _M_start + __new_size;
    } else {
    }
  }
  void resize(size_t __new_size) { resize(__new_size, _Tp()); }
};
