#ifndef CONSTRUCT_H_
#define CONSTRUCT_H_

#include "include/type_traits.h"

#include <iterator>

namespace gc_impl {

template <typename T1, typename T2>
inline void Construct(T1* p, const T2& value) {
  new (static_cast<void*>(p)) T1(value);
}

template <typename T1>
inline void Construct(T1* p) {
  new (static_cast<void*>(p)) T1();
}

template <typename Tp>
inline void Destroy(Tp* pointer) {
  pointer->~Tp();
}

template <typename ForwardIterator>
inline void destroy_aux(ForwardIterator first, ForwardIterator last,
                        false_type) {
  for (; first != last; ++first) {
    Destroy(&*first);
  }
}

template <typename ForwardIterator>
inline void destroy_aux(ForwardIterator first, ForwardIterator last,
                        true_type) {}

template <typename ForwardIterator>
inline void Destroy(ForwardIterator first, ForwardIterator last) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type Value_type;
  typedef typename type_traits<Value_type>::is_pod_type Is_pod_type;
  destroy_aux(first, last, Is_pod_type());
}

}  // namespace gc_impl

#endif
