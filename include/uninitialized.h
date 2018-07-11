#ifndef UNINITIALIZED_H_
#define UNINITIALIZED_H_

#include <string.h>

#include <iterator>
#include <utility>

#include "include/construct.h"
#include "include/gtype_traits.h"

template <typename _InputIterator, typename _ForwardIterator>
inline _ForwardIterator __uninitialized_gcopy_aux(_InputIterator __first,
                                                  _InputIterator __last,
                                                  _ForwardIterator __result,
                                                  __true_gtype) {
  return std::copy(__first, __last, __result);
}

template <typename _InputIterator, typename _ForwardIterator>
inline _ForwardIterator __uninitialized_gcopy_aux(_InputIterator __first,
                                                  _InputIterator __last,
                                                  _ForwardIterator __result,
                                                  __false_gtype) {
  _ForwardIterator __cur = __result;
  for (; __first != __last; ++__first, ++__cur) {
    _GConstruct(&*__cur, *__first);
  }
  return __cur;
}

template <typename _InputIterator, typename _ForwardIterator>
inline _ForwardIterator uninitialized_gcopy(_InputIterator __first,
                                            _InputIterator __last,
                                            _ForwardIterator __result) {
  typedef typename std::iterator_traits<_ForwardIterator>::value_type _Value_type;
  typedef typename __gtype_traits<_Value_type>::is_pod_type _Is_pod_type;
  return __uninitialized_gcopy_aux(
      __first, __last, __result,
      _Is_pod_type());
      // __gtype_traits<
          // std::iterator_traits<_ForwardIterator>::value_type>::is_pod_type());
}

inline char* uninitialized_gcopy(const char* __first, const char* __last,
                                 char* __result) {
  memmove(__result, __first, __last - __first);
  return __result + (__last - __first);
}

template <typename _ForwardIterator, typename _Size, typename _Tp>
inline _ForwardIterator __uninitialized_gfill_n_aux(_ForwardIterator __first,
                                                    _Size __n, const _Tp& __x,
                                                    __true_gtype) {
  return std::fill_n(__first, __n, __x);
}

template <typename _ForwardIterator, typename _Size, typename _Tp>
inline _ForwardIterator __uninitialized_gfill_n_aux(_ForwardIterator __first,
                                                    _Size __n, const _Tp& __x,
                                                    __false_gtype) {
  _ForwardIterator __cur = __first;
  for (; __n > 0; --__n, ++__cur) {
    _GConstruct(&*__cur, __x);
  }
  return __cur;
}

template <typename _ForwardIterator, typename _Size, typename _Tp>
inline _ForwardIterator uninitialized_gfill_n(_ForwardIterator __first, _Size __n,
                                  const _Tp& __x) {
  typedef typename std::iterator_traits<_ForwardIterator>::value_type _Value_type;
  typedef typename __gtype_traits<_Value_type>::is_pod_type _Is_pod_type;
  return __uninitialized_gfill_n_aux(__first, __n, __x, _Is_pod_type());
      // __gtype_traits<
          // std::iterator_traits<_ForwardIterator>::value_type>::is_pod_type());
}
#endif
