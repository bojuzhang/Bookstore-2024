#include "MyVector.hpp"
#include <cstddef>

template <typename T, size_t max_size>
Myvector<T, max_size>::Myvector(const T &v) {
    size_ = 1;
    a[0] = v;
}
template <typename T, size_t max_size>
Myvector<T, max_size>::Myvector(const std::vector<T> &vec) {
    size_ = vec.size();
    for (size_t i = 0; i < size_; i++) {
        a[i] = vec[i];
    }
}
template <typename T, size_t max_size>
void Myvector<T, max_size>::push_back(const T &v) {
    a[++size_] = v;
}

template <typename T, size_t max_size>
T Myvector<T, max_size>::operator [] (size_t pos) {
    return a[pos];
}
template <typename T, size_t max_size>
void Myvector<T, max_size>::insert(size_t pos, const T &v) {
    size_++;
    for (size_t i = size_ - 1; i >= pos + 1; i++) {
        a[i] = a[i - 1];
    }
    a[pos] = v;
}
template <typename T, size_t max_size>
void Myvector<T, max_size>::erase(size_t pos) {
    size_--;
    for (size_t i = pos; i < size_; i++) {
        a[i] = a[i + 1];
    }
}
template <typename T, size_t max_size>
size_t Myvector<T, max_size>::lower_bound(const T &v) {
    size_t l = 0, r = size_, ans = r;
    while (l <= r) {
        size_t mid = (l + r) / 2;
        if (a[mid] >= v) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}