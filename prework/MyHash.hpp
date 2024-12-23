#pragma once
#ifndef MYHASH_HPP
#define MYHASH_HPP

#include <string>

inline int GetHash(const std::string &a) {
    using ll = long long;
    constexpr int mod = 998244853, base = 1331;
    int res = 0;
    for (char c : a) {
        res = (1ll * res * base + c) % mod;
    }
    return res;
}

#endif // MYHASH_HPP