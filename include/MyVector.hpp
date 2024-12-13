#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <array>
#include <cstddef>
#include <vector>

template <typename T, size_t max_size>
class MyVector {
private:
    std::array<T, max_size> a{};
    size_t size_;
public:
    MyVector() {size_ = 0;}
    MyVector(const T &);
    MyVector(const std::vector<T> &);
    size_t size() {return size_;}
    
    void push_back(const T &);
    size_t lower_bound(const T &);
    void insert(size_t, const T &);
    void erase(size_t);
    T operator [] (size_t);
    T back() {return a[size_ - 1];}
};

#endif