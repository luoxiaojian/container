#ifndef CONSTRUCT_H_
#define CONSTRUCT_H_

#include "gtype_traits.h"

#include <iterator>

template <typename _T1, typename _T2>
inline void _GConstruct(_T1* __p, const _T2& __value) {
  new (static_cast<void*>(__p)) __T1(__value);
}

template <typename _T1>
inline void _GConstruct(_T1* __p) {
  new (static_cast<void*>(__p)) _T1();
}

template <typename _Tp>
inline void _Destroy(_Tp* __pointer) {
  __pointer->~_Tp();
}

template <typename _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
                          __false_gtype) {
  for (; __first != __last; ++__first) {
    _Destroy(&*__first);
  }
}

template <typename _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
                          __false_gtype) {
  for (; __first != __last; ++__first) {
    _Destroy(&*__first);
  }
}

template <typename _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
                          __true_gtype) {}

template <typename _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) {
  __destroy_aux(
      __first, __last,
      __gtype_traits<
          std::iterator_traits<_ForwardIterator>::value_type>::is_pod_type());
}

#endif
