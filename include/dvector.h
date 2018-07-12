#ifndef DVECTOR_H_
#define DVECTOR_H_

#include <algorithm>
#include <string>

#include "include/gcontainer.h"

enum DVecState {
  DVecAllocated,
  DVecMapped,
};

template <typename T>
class DVector : public gvector<T> {
 public:
  DVector() : gvector<T>(), state_(DVecAllocated) {}
  explicit DVector(size_t size) : gvector<T>(size), state_(DVecAllocated) {}
  DVector(size_t size, const T& value)
      : gvector<T>(size, value), state_(DVecAllocated) {}

  ~DVector() {
    if (state_ == DVecMapped) {
      Release();
    }
  }

  void Clear() {
    if (state_ == DVecMapped) {
      Release();
    } else {
      gvector<T>::Clear();
    }
  }

  void Resize(size_t n, const T& value) {
    if (state_ == DVecAllocated) {
      gvector<T>::Resize(n, value);
    }
  }

  void Resize(size_t n) {
    if (state_ == DVecAllocated) {
      gvector<T>::Resize(n);
    }
  }

  void Dump(const std::string& fname) {
    FILE* fout = fopen(fname.c_str(), "wb");
    fwrite(start_, sizeof(T), Size(), fout);
    fflush(fout);
    fclose(fout);
  }

  void Load(const std::string& fname) {
    if (state_ == DVecMapped) {
      Release();
    } else {
      if (start_ != NULL) {
        GFREE(start_, Capacity() * sizeof(T));
      }
    }
    start_ = finish_ = end_of_storage_ = NULL;
    state_ = DVecAllocated;

    int fd = open(fname.c_str(), O_RDONLY);
    if (fd == -1) {
      return;
    }

    struct stat = st;
    fstat(fd, &st);
    size_t file_size = st.st_size;
    size_t size = file_size / sizeof(T);
    start_ = reinterpret_cast<T*>(
        mmap(NULL, size * sizeof(T), PROT_READ, MAP_PRIVATE, fd, 0));
    finish_ = end_of_storage_ = start_ + size;
    close(fd);
    state_ = DVecMapped;
  }

  void Release() {
    if (state_ == DVecAllocated) {
      return;
    }
    munmap(start_, Size() * sizeof(T));
    start_ = finish_ = end_of_storage_ = NULL;
    state_ = DVecAllocated;
  }

  void Access() {
    if (state_ == DVecAllocated) {
      return;
    }
    size_t size = sizeof(T) * Size();
    size_t size_in_bytes = sizeof(T) * size;
    T* new_start = reinterpret_cast<T*>(GMALLOC(size_in_bytes));
    memcpy(new_start, start_, size_in_bytes);
    Release();
    start_ = new_start;
    finish_ = end_of_storage_ = new_start + size;
  }

  void PushBack(const T& value) {
    if (state_ == DVecAllocated) {
      gvector<T>::PushBack(value);
    }
  }

 private:
  DVecState state_;
};

#endif  // DVECTOR_H_
