#ifndef DVECTOR_H_
#define DVECTOR_H_

#include <algorithm>
#include <string>
#include <type_traits>

#include "include/gcontainer.h"
#include "include/type_traits.h"

enum DCState {
  DCAllocated,
  DCMapped,
};

template <typename T>
class DVector : public GVector<T> {
  static_assert(std::is_same<gc_impl::true_type,
                             gc_impl::type_traits<T>::is_pod_type>::value,
                "DVector should based on pod type");

 public:
  DVector() : GVector<T>(), state_(DCAllocated) {}
  explicit DVector(size_t size) : GVector<T>(size), state_(DCAllocated) {}
  DVector(size_t size, const T& value)
      : GVector<T>(size, value), state_(DCAllocated) {}

  ~DVector() {
    if (state_ == DCMapped) {
      Release();
    }
  }

  void Clear() {
    if (state_ == DCMapped) {
      Release();
    } else {
      GVector<T>::Clear();
    }
  }

  void Resize(size_t n, const T& value) {
    if (state_ == DCAllocated) {
      GVector<T>::Resize(n, value);
    }
  }

  void Resize(size_t n) {
    if (state_ == DCAllocated) {
      GVector<T>::Resize(n);
    }
  }

  void Dump(const std::string& fname) {
    FILE* fout = fopen(fname.c_str(), "wb");
    fwrite(GVector<T>::start_, sizeof(T), Size(), fout);
    fflush(fout);
    fclose(fout);
  }

  void Load(const std::string& fname) {
    Clear();

    int fd = open(fname.c_str(), O_RDONLY);
    if (fd == -1) {
      return;
    }

    struct stat = st;
    fstat(fd, &st);
    size_t file_size = st.st_size;
    size_t size = file_size / sizeof(T);
    GVector<T>::start_ = reinterpret_cast<T*>(
        mmap(NULL, size * sizeof(T), PROT_READ, MAP_PRIVATE, fd, 0));
    GVector<T>::finish_ = GVector<T>::end_of_storage_ =
        GVector<T>::start_ + size;
    close(fd);
    state_ = DCMapped;
  }

  void Release() {
    if (state_ == DCAllocated) {
      return;
    }
    munmap(GVector<T>::start_, Size() * sizeof(T));
    GVector<T>::start_ = GVector<T>::finish_ = GVector<T>::end_of_storage_ =
        NULL;
    state_ = DCAllocated;
  }

  void Access() {
    if (state_ == DCAllocated) {
      return;
    }
    size_t size = Size();
    size_t size_in_bytes = sizeof(T) * size;
    T* new_start = reinterpret_cast<T*>(GMALLOC(size_in_bytes));
    memcpy(new_start, GVector<T>::start_, size_in_bytes);
    Release();
    GVector<T>::start_ = new_start;
    GVector<T>::finish_ = GVector<T>::end_of_storage_ = new_start + size;
  }

  void PushBack(const T& value) {
    if (state_ == DCAllocated) {
      GVector<T>::PushBack(value);
    }
  }

 private:
  DCState state_;
};

template <typename T>
class DArray : public GArray<T> {
  static_assert(std::is_same<gc_impl::true_type,
                             gc_impl::type_traits<T>::is_pod_type>::value,
                "DArray should based on pod type");

 public:
  DArray() : GArray<T>(), state_(DCAllocated) {}
  explicit DArray(size_t size) : GArray<T>(size), state_(DCAllocated) {}
  DArray(size_t size, const T& value)
      : GArray<T>(size, value), state_(DCAllocated) {}

  ~DArray() {
    if (state_ == DCMapped) {
      Release();
    }
  }

  void Clear() {
    if (state_ == DCMapped) {
      Release();
    } else {
      GArray<T>::Clear();
    }
  }

  void Resize(size_t n, const T& value) {
    if (state_ == DCAllocated) {
      GArray<T>::Resize(n, value);
    }
  }

  void Resize(size_t n) {
    if (state_ == DCAllocated) {
      GArray<T>::Resize(n);
    }
  }

  void Dump(const std::string& fname) {
    FILE* fout = fopen(fname.c_str(), "wb");
    fwrite(GArray<T>::start_, sizeof(T), Size(), fout);
    fflush(fout);
    fclose(fout);
  }

  void Load(const std::string& fname) {
    Clear();
    int fd = open(fname.c_str(), O_RDONLY);
    if (fd == -1) {
      return;
    }

    struct stat = st;
    fstat(fd, &st);
    size_t file_size = st.st_size;
    size_t size = file_size / sizeof(T);
    GArray<T>::start_ = reinterpret_cast<T*>(
        mmap(NULL, size * sizeof(T), PROT_READ, MAP_PRIVATE, fd, 0));
    GArray<T>::finish_ = GArray<T>::start_ + size;
    close(fd);
    state_ = DCMapped;
  }

  void Release() {
    if (state_ == DCAllocated) {
      return;
    }
    munmap(GArray<T>::start_, Size() * sizeof(T));
    GArray<T>::start_ = GArray<T>::finish_ = NULL;
    state_ = DCAllocated;
  }

  void Access() {
    if (state_ == DCAllocated) {
      return;
    }
    size_t size = Size();
    size_t size_in_bytes = sizeof(T) * size;
    T* new_start = reinterpret_cast<T*>(GMALLOC(size_in_bytes));
    memcpy(new_start, GArray<T>::start_, size_in_bytes);
    Release();
    GArray<T>::start_ = new_start;
    GArray<T>::finish_ = new_start + size;
  }

 private:
  DCState state_;
};

#endif  // DVECTOR_H_
