#ifndef CONSTRUCT_H_
#define CONSTRUCT_H_

#include "include/gtype_traits.h"

#include <iterator>

template <typename T1, typename T2>
inline void _GConstruct(T1* p, const T2& value) {
  new (static_cast<void*>(p)) T1(value);
}

template <typename T1>
inline void _GConstruct(T1* p) {
  new (static_cast<void*>(p)) T1();
}

template <typename Tp>
inline void _GDestroy(Tp* pointer) {
  pointer->~Tp();
}

template <typename ForwardIterator>
inline void destroy_aux(ForwardIterator first, ForwardIterator last,
                        false_gtype) {
  for (; first != last; ++first) {
    _GDestroy(&*first);
  }
}

template <typename ForwardIterator>
inline void destroy_aux(ForwardIterator first, ForwardIterator last,
                        true_gtype) {}

template <typename ForwardIterator>
inline void _GDestroy(ForwardIterator first, ForwardIterator last) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename gtype_traits<Value_type>::is_pod_type Is_pod_type;
  destroy_aux(first, last, Is_pod_type());
}

#endif
