#ifndef UNINITIALIZED_H_
#define UNINITIALIZED_H_

#include <string.h>

#include <iterator>
#include <utility>

#include "include/construct.h"
#include "include/type_traits.h"

namespace gc_impl {

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy_aux(InputIterator first,
                                              InputIterator last,
                                              ForwardIterator result,
                                              true_type) {
  return std::copy(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy_aux(InputIterator first,
                                              InputIterator last,
                                              ForwardIterator result,
                                              false_type) {
  ForwardIterator cur = result;
  for (; first != last; ++first, ++cur) {
    Construct(&*cur, *first);
  }
  return cur;
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first,
                                          InputIterator last,
                                          ForwardIterator result) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename type_traits<Value_type>::is_pod_type Is_pod_type;
  return uninitialized_copy_aux(first, last, result, Is_pod_type());
}

inline char* uninitialized_copy(const char* first, const char* last,
                                char* result) {
  memmove(result, first, last - first);
  return result + (last - first);
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, Size n,
                                                const Tp& x, true_type) {
  return std::fill_n(first, n, x);
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, Size n,
                                                const Tp& x, false_type) {
  ForwardIterator cur = first;
  for (; n > 0; --n, ++cur) {
    Construct(&*cur, x);
  }
  return cur;
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_gfill_n(ForwardIterator first, Size n,
                                             const Tp& x) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename type_traits<Value_type>::is_pod_type Is_pod_type;
  return uninitialized_fill_n_aux(first, n, x, Is_pod_type());
}

template <typename ForwardIterator, typename Size>
inline ForwardIterator uninitialized_fill_n_nv_aux(ForwardIterator first,
                                                   Size n, true_type) {
  return first + n;
}
template <typename ForwardIterator, typename Size>
inline ForwardIterator uninitialized_fill_n_nv_aux(ForwardIterator first,
                                                   Size n, false_type) {
  ForwardIterator cur = first;
  for (; n > 0; --n, ++cur) {
    Construct(&*cur);
  }
  return cur;
}

template <typename ForwardIterator, typename Size>
inline ForwardIterator uninitialized_fill_n_nv(ForwardIterator first, Size n) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename type_traits<Value_type>::is_pod_type Is_pod_type;
  return uninitialized_fill_n_nv_aux(first, n, Is_pod_type());
}

}  // namespace gc_impl
#endif
