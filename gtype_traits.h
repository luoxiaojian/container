#ifndef GTYPE_TRAITS_H_
#define GTYPE_TRAITS_H_

struct __true_gtype {};
struct __false_gtype {};

template <class _Tp>
struct __gtype_traits {
  typedef __false_gtype is_pod_type;
};

#define DECLARE_TT(type, vt1, tt1) \
  template <>                      \
  struct __gtype_traits<type> {    \
    typedef vt1 tt1;               \
  };

DECLARE_TT(bool, __true_gtype, is_pod_type)
DECLARE_TT(char, __true_gtype, is_pod_type)
DECLARE_TT(signed char, __true_gtype, is_pod_type)
DECLARE_TT(unsigned char, __true_gtype, is_pod_type)
DECLARE_TT(short, __true_gtype, is_pod_type)
DECLARE_TT(unsigned short, __true_gtype, is_pod_type)
DECLARE_TT(int, __true_gtype, is_pod_type)
DECLARE_TT(unsigned int, __true_gtype, is_pod_type)
DECLARE_TT(long, __true_gtype, is_pod_type)
DECLARE_TT(unsigned long, __true_gtype, is_pod_type)
DECLARE_TT(long long, __true_gtype, is_pod_type)
DECLARE_TT(unsigned long long, __true_gtype, is_pod_type)
DECLARE_TT(float, __true_gtype, is_pod_type)
DECLARE_TT(double, __true_gtype, is_pod_type)
DECLARE_TT(long double, __true_gtype, is_pod_type)

template <class _Tp>
struct __gtype_traits<_Tp*> {
  typedef __true_gtype is_pod_type;
};

#endif
