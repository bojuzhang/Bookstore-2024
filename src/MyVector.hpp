#pragma once
#include <iostream>
#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <array>
#include <cstddef>
#include <vector>

template <class T, size_t max_size>
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
    T& operator [] (size_t);
    const T& operator [] (size_t) const;
    T& back() {return a[size_ - 1];}
    const T& back() const {return a[size_ - 1];}

    void print() {
        std::cerr << "size: " << size_ << "\n";
        for (size_t i = 0; i < size_; i++) {
            std::cerr << a[i] << "\n";
        }
    };
};

template <typename T, size_t max_size>
MyVector<T, max_size>::MyVector(const T &v) {
    size_ = 1;
    a[0] = v;
}
template <typename T, size_t max_size>
MyVector<T, max_size>::MyVector(const std::vector<T> &vec) {
    size_ = vec.size();
    for (size_t i = 0; i < size_; i++) {
        a[i] = vec[i];
    }
}
template <typename T, size_t max_size>
void MyVector<T, max_size>::push_back(const T &v) {
    a[size_++] = v;
}

template <typename T, size_t max_size>
T& MyVector<T, max_size>::operator [] (size_t pos) {
    return a[pos];
}
template <typename T, size_t max_size>
const T& MyVector<T, max_size>::operator [] (size_t pos) const {
    return a[pos];
}
template <typename T, size_t max_size>
void MyVector<T, max_size>::insert(size_t pos, const T &v) {
    size_++;
    for (size_t i = size_ - 1; i >= pos + 1; i--) {
        a[i] = a[i - 1];
    }
    a[pos] = v;
}
template <typename T, size_t max_size>
void MyVector<T, max_size>::erase(size_t pos) {
    size_--;
    for (size_t i = pos; i < size_; i++) {
        a[i] = a[i + 1];
    }
}
template <typename T, size_t max_size>
size_t MyVector<T, max_size>::lower_bound(const T &v) {
    int l = 0, r = size_, ans = r;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (a[mid] >= v) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

// TEST_CASE("MyVector is correct", "[MyVector]") {
//     MyVector<int, 100> ve;
//     ve.push_back(-10);
//     ve.push_back(6);
//     ve.push_back(114514);
//     SECTION("push_back is correct") {
//         REQUIRE(ve[0] == -10);
//         REQUIRE(ve[1] == 6);
//         REQUIRE(ve[2] == 114514);
//     }
//     SECTION("insert is correct") {
//         ve.insert(2, 1919810);
//         REQUIRE(ve[2] == 1919810);
//     }
//     SECTION("lower_bound is correct") {
//         REQUIRE(ve.lower_bound(8) == 2);
//         REQUIRE(ve.lower_bound(1919810) == 3);
//         REQUIRE(ve.lower_bound(6) == 1);
//         ve.insert(ve.lower_bound(9), 9);
//         REQUIRE(ve[2] == 9);
//         ve.insert(ve.lower_bound(-1919810), -1919810);
//         REQUIRE(ve[0] == -1919810);
//     }
//     SECTION("erase is correct") {
//         ve.erase(ve.lower_bound(-10));
//         REQUIRE(ve[0] == 6);
//     }
// }

#endif