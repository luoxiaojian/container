#ifndef UNINITIALIZED_H_
#define UNINITIALIZED_H_

#include <string.h>

#include <iterator>
#include <utility>

#include "include/construct.h"
#include "include/gtype_traits.h"

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_gcopy_aux(InputIterator first,
                                               InputIterator last,
                                               ForwardIterator result,
                                               true_gtype) {
  return std::copy(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_gcopy_aux(InputIterator first,
                                               InputIterator last,
                                               ForwardIterator result,
                                               false_gtype) {
  ForwardIterator cur = result;
  for (; first != last; ++first, ++cur) {
    _GConstruct(&*cur, *first);
  }
  return cur;
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_gcopy(InputIterator first,
                                           InputIterator last,
                                           ForwardIterator result) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename gtype_traits<Value_type>::is_pod_type Is_pod_type;
  return uninitialized_gcopy_aux(first, last, result, Is_pod_type());
}

inline char* uninitialized_gcopy(const char* first, const char* last,
                                 char* result) {
  memmove(result, first, last - first);
  return result + (last - first);
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_gfill_n_aux(ForwardIterator first, Size n,
                                                 const Tp& x, true_gtype) {
  return std::fill_n(first, n, x);
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_gfill_n_aux(ForwardIterator first, Size n,
                                                 const Tp& x, false_gtype) {
  ForwardIterator cur = first;
  for (; n > 0; --n, ++cur) {
    _GConstruct(&*cur, x);
  }
  return cur;
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_gfill_n(ForwardIterator first, Size n,
                                             const Tp& x) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename gtype_traits<Value_type>::is_pod_type Is_pod_type;
  return uninitialized_gfill_n_aux(first, n, x, Is_pod_type());
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_gfill2_n_aux(ForwardIterator first, Size n,
                                                 const Tp& x, true_gtype) {
  return first + n;
  // return std::fill_n(first, n, x);
}
template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_gfill2_n_aux(ForwardIterator first, Size n,
                                                 const Tp& x, false_gtype) {
  ForwardIterator cur = first;
  for (; n > 0; --n, ++cur) {
    _GConstruct(&*cur, x);
  }
  return cur;
}

template <typename ForwardIterator, typename Size, typename Tp>
inline ForwardIterator uninitialized_gfill2_n(ForwardIterator first, Size n,
                                             const Tp& x) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename gtype_traits<Value_type>::is_pod_type Is_pod_type;
  return uninitialized_gfill2_n_aux(first, n, x, Is_pod_type());
}
#endif
