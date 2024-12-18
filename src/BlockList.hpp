#pragma once
#ifndef BLOCKLIST_HPP
#define BLOCKLIST_HPP

#include "MemoryRiver.hpp"
#include "MyVector.hpp"
#include <cstddef>
#include <utility>
#include <vector>

template <class Tkey, class Tvalue, size_t max_size = 100000, size_t block_size = 30>
class BlockList {
private:
    // 存储每一个 BlockNode 的信息，并记录下一个在哪个位置（链表）
    struct HeadNode {
        std::pair<Tkey, Tvalue> head_;
        std::pair<Tkey, Tvalue> end_;
        size_t size_;
        size_t nxt_, pos_;
    };
    /* 块状链表的一块，存储所有键值对

    在文件上存储，采用 MyVector 代替链表*/
    using BlockNode = MyVector<std::pair<Tkey, Tvalue>, block_size * 2>;
    using HeadList = MyVector<HeadNode, max_size / block_size * 2>;

    HeadList headlist_;

    MemoryRiver<HeadList> headmemory_;
    MemoryRiver<BlockNode> nodememory_;

    size_t size_, len_;

public:
    BlockList() = delete;
    BlockList(const std::string &);
    ~BlockList();

    void BlockInsert(HeadNode &, const std::pair<Tkey, Tvalue> &, size_t);
    void BlockDelete(HeadNode &, const std::pair<Tkey, Tvalue> &);
    void BlockModify(HeadNode &, const MyVector<std::pair<Tkey, Tvalue>, block_size * 2> &);
    std::vector<Tvalue> BlockFind(HeadNode &, const Tkey &);

    size_t FindPos(const std::pair<Tkey, Tvalue> &);

    void Insert(const std::pair<Tkey, Tvalue> &);
    void Delete(const std::pair<Tkey, Tvalue> &);
    std::vector<Tvalue> Find(const Tkey &);
    size_t size() {return size_;}
};

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
BlockList<Tkey, Tvalue, max_size, block_size>::BlockList(const std::string &file_name) {
    headmemory_.initialise(file_name + "_head", 1);
    nodememory_.initialise(file_name + "_node", 1);
    
    int t = 0;
    headmemory_.get_info(t, 2);
    size_ = t;
    headmemory_.get_info(t, 1);
    if (t != 0) {
        headmemory_.read(headlist_, 0);
        len_ = headlist_.size();
    }
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
BlockList<Tkey, Tvalue, max_size, block_size>::~BlockList() {
    headmemory_.write_info(size_, 2);
    headmemory_.write_info(headlist_[0].pos_, 3);
    headmemory_.write_info(len_, 4);
    int t = 0;
    headmemory_.get_info(t, 1);
    if (t == 0) {
        headmemory_.write(headlist_);
    } else {
        headmemory_.update(headlist_, 0);
    }
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockInsert
(HeadNode &cur, const std::pair<Tkey, Tvalue> &v, size_t headnodepos) {
    BlockNode vec;
    nodememory_.read(vec, cur.pos_);
    vec.insert(vec.lower_bound(v), v);
    if (vec.size() > block_size) {
        BlockNode L, R;
        size_t half = vec.size() / 2;
        for (size_t i = 0; i < half; i++) {
            L.push_back(vec[i]);
        }
        for (size_t i = half; i < vec.size(); i++) {
            R.push_back(vec[i]);
        }
        size_t pos = nodememory_.write(R);
        nodememory_.update(L, cur.pos_);
        HeadNode p = {R[0], R.back(), R.size(), cur.nxt_, pos};
        cur = {L[0], L.back(), L.size(), pos, cur.pos_};
        headlist_.insert(headnodepos + 1, p);
        BlockNode p1, p2;
        nodememory_.read(p1, headlist_[headnodepos].pos_);
        nodememory_.read(p2, headlist_[headnodepos + 1].pos_);
        ++len_;
    } else {
        nodememory_.update(vec, cur.pos_);
        cur = {vec[0], vec.back(), vec.size(), cur.nxt_, cur.pos_};
    }
}
template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockModify
(HeadNode &cur, const MyVector<std::pair<Tkey, Tvalue>, block_size * 2> &v) {
    BlockNode vec;
    nodememory_.read(vec, cur.pos_);
    vec = v;
    nodememory_.update(vec, cur.pos_);
    cur = {vec[0], vec.back(), vec.size(), cur.nxt_, cur.pos_};
}
template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockDelete
(HeadNode &cur, const std::pair<Tkey, Tvalue> &v) {
    MyVector<std::pair<Tkey, Tvalue>, block_size * 2> vec;
    nodememory_.read(vec, cur.pos_);
    auto p = vec.lower_bound(v);
    if (p == size_ || vec[p] != v) {
        return;
    }
    vec.erase(p);
    nodememory_.update(vec, cur.pos_);
    cur = {vec[0], vec.back(), vec.size(), cur.nxt_, cur.pos_};
    --size_;
}
template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
std::vector<Tvalue> BlockList<Tkey, Tvalue, max_size, block_size>::BlockFind
(HeadNode &cur, const Tkey &key) {
    BlockNode vec;
    nodememory_.read(vec, cur.pos_);
    std::vector<Tvalue> res;
    for (size_t i = 0; i < vec.size(); i++) {
        auto x = vec[i];
        if (x.first == key) {
            res.push_back(x.second);
        }
    }
    return res;
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
size_t BlockList<Tkey, Tvalue, max_size, block_size>::FindPos(const std::pair<Tkey, Tvalue> &v) {
    int l = 0, r = len_ - 1, ans = len_;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (headlist_[mid].head_ > v) {
            r = mid - 1;
            ans = mid;
        } else {
            l = mid + 1;
        }
    }
    if (ans == 0) return 0;
    else return ans - 1;
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::Insert
(const std::pair<Tkey, Tvalue> &v) {
    if (size_ == 0) {
        auto p = BlockNode(v);
        auto pos = nodememory_.write(p);
        len_ = 1;
        headlist_ = HeadList(HeadNode{v, v, 1, 0, pos});
    } else {
        int pos = FindPos(v);
        BlockInsert(headlist_[pos], v, pos);
    }
    ++size_;
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::Delete
(const std::pair<Tkey, Tvalue> &v) {
    if (size_ == 0) {
        return;
    } 
    int pos = FindPos(v);
    if (headlist_[pos].head_ <= v && v <= headlist_[pos].end_) {
        BlockDelete(headlist_[pos], v);
    }
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
std::vector<Tvalue> BlockList<Tkey, Tvalue, max_size, block_size>::Find(const Tkey &key) {
    if (size_ == 0) {
        return {};
    } 
    std::vector<Tvalue> res;
    for (size_t i = 0; i < len_; i++) {
        if (headlist_[i].head_.first <= key && key <= headlist_[i].end_.first) {
            auto vec = BlockFind(headlist_[i], key);
            for (size_t i = 0; i < vec.size(); i++) {
                res.push_back(vec[i]);
            }
        }
        if (headlist_[i].head_.first > key) {
            break;
        }
    }
    return res;
}

#endif // BLOCKLIST_HPP