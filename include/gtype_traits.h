#ifndef GTYPE_TRAITS_H_
#define GTYPE_TRAITS_H_

struct true_gtype {};
struct false_gtype {};

template <class _Tp>
struct gtype_traits {
  typedef false_gtype is_pod_type;
};

#define DECLARE_TT(type, vt1, tt1) \
  template <>                      \
  struct gtype_traits<type> {      \
    typedef vt1 tt1;               \
  };

DECLARE_TT(bool, true_gtype, is_pod_type)
DECLARE_TT(char, true_gtype, is_pod_type)
DECLARE_TT(signed char, true_gtype, is_pod_type)
DECLARE_TT(unsigned char, true_gtype, is_pod_type)
DECLARE_TT(short, true_gtype, is_pod_type)
DECLARE_TT(unsigned short, true_gtype, is_pod_type)
DECLARE_TT(int, true_gtype, is_pod_type)
DECLARE_TT(unsigned int, true_gtype, is_pod_type)
DECLARE_TT(long, true_gtype, is_pod_type)
DECLARE_TT(unsigned long, true_gtype, is_pod_type)
DECLARE_TT(long long, true_gtype, is_pod_type)
DECLARE_TT(unsigned long long, true_gtype, is_pod_type)
DECLARE_TT(float, true_gtype, is_pod_type)
DECLARE_TT(double, true_gtype, is_pod_type)
DECLARE_TT(long double, true_gtype, is_pod_type)

template <class _Tp>
struct gtype_traits<_Tp*> {
  typedef true_gtype is_pod_type;
};

#endif
