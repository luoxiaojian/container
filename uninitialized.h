#ifndef UNINITIALIZED_H_
#define UNINITIALIZED_H_

#include <string.h>

#include <iterator>
#include <utility>

#include "construct.h"
#include "gtype_traits.h"

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
  return __uninitialized_gcopy_aux(
      __first, __last, __result,
      __gtype_traits<
          std::iterator_traits<_ForwardIterator>::value_type>::is_pod_type());
}

inline char* uninitialized_gcopy(const char* __first, const char* __last,
                                 char* __result) {
  memmove(__result, __first, __last - __first);
  return __result + (__last - __first);
}

template <typename _ForwardIterator, typename _Tp>
inline void __uninitialized_gfill_aux(_ForwardIterator __first,
                                      _ForwardIterator __last, const _Tp& __x,
                                      __true_gtype) {
  std::fill(__first, __last, __x);
}

template <typename _ForwardIterator, typename _Tp>
inline void __uninitialized_gfill_aux(_ForwardIterator __first,
                                      _ForwardIterator __last, const _Tp& __x,
                                      __false_gtype) {
  _ForwardIterator __cur = __first;
  for (; __cur != __last; ++__cur) {
    _GConstruct(&*__cur, __x);
  }
}

template <typename _ForwardIterator, typename _Tp>
inline void uninitialized_gfill(_ForwardIterator __first,
                                _ForwardIterator __last, const _Tp& __x) {
  __uninitialized_gfill_aux(
      __first, __last,
      __gtype_traits<
          std::iterator_traits<_ForwardIterator>::value_type>::is_pod_type());
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
inline void uninitialized_gfill_n(_ForwardIterator __first, _Size __n,
                                  const _Tp& __x) {
  __uninitialized_gfill_n_aux(
      __first, __n,
      __gtype_traits<
          std::iterator_traits<_ForwardIterator>::value_type>::is_pod_type());
}
#endif
