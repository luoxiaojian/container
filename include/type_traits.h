#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

namespace gc_impl {

struct true_type {};
struct false_type {};

template <class _Tp>
struct type_traits {
  typedef false_type is_pod_type;
};

#define DECLARE_TT(type, vt1, tt1) \
  template <>                      \
  struct type_traits<type> {       \
    typedef vt1 tt1;               \
  };

DECLARE_TT(bool, true_type, is_pod_type)
DECLARE_TT(char, true_type, is_pod_type)
DECLARE_TT(signed char, true_type, is_pod_type)
DECLARE_TT(unsigned char, true_type, is_pod_type)
DECLARE_TT(short, true_type, is_pod_type)
DECLARE_TT(unsigned short, true_type, is_pod_type)
DECLARE_TT(int, true_type, is_pod_type)
DECLARE_TT(unsigned int, true_type, is_pod_type)
DECLARE_TT(long, true_type, is_pod_type)
DECLARE_TT(unsigned long, true_type, is_pod_type)
DECLARE_TT(long long, true_type, is_pod_type)
DECLARE_TT(unsigned long long, true_type, is_pod_type)
DECLARE_TT(float, true_type, is_pod_type)
DECLARE_TT(double, true_type, is_pod_type)
DECLARE_TT(long double, true_type, is_pod_type)

template <class _Tp>
struct type_traits<_Tp*> {
  typedef true_type is_pod_type;
};

}  // namespace gc_impl

#endif
