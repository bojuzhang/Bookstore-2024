#pragma once
#include <iostream>
#ifndef BLOCKLIST_HPP
#define BLOCKLIST_HPP

#include "MemoryRiver.hpp"
#include "MyVector.hpp"
#include <cstddef>
#include <utility>
#include <vector>

template <class Tkey, class Tvalue, size_t max_size = 100000, size_t block_size = 350>
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

    在文件上存储，采用 std::vector 代替链表*/
    typedef MyVector<std::pair<Tkey, Tvalue>, block_size * 2> BlockNode;
    /* 用于存储从文件中转写下来的 HeadNode 链表，在析构时写入文件中

    采用手写链表：内存限制（相比std::vector），利于实现插入删除（相比std::array）*/
    struct HeadList {
        HeadNode node_;
        size_t pos_;
        HeadList *nxt_{};
        HeadList(HeadNode node, size_t pos) : node_(node), pos_(pos) { };
    };

    HeadList* head_;

    MemoryRiver<HeadNode> headmemory_;
    MemoryRiver<BlockNode> nodememory_;

    size_t size_;

public:
    BlockList() = delete;
    BlockList(const std::string &);
    ~BlockList();

    void BlockInsert(HeadList *, const std::pair<Tkey, Tvalue> &);
    void BlockDelete(HeadList *, const std::pair<Tkey, Tvalue> &);
    void BlockModify(HeadList *, const MyVector<std::pair<Tkey, Tvalue>, block_size * 2> &);
    std::vector<Tvalue> BlockFind(HeadList *, const Tkey &);

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
    // std::cerr << size_ << "\n";

    if (size_) {
        headmemory_.get_info(t, 3);
        size_t head_pos = t;
        // std::cerr << "test " << head_pos << " " << t << "\n";
        HeadNode node;
        headmemory_.read(node, head_pos);
        // std::cerr << node.pos_ << " " << node.head_.second << " " << node.end_.second << " " << node.nxt_ << "\n";
        head_ = new HeadList(node, node.pos_);
        auto cur = head_;
        while (node.nxt_) {
            headmemory_.read(node, node.nxt_);
            auto p = new HeadList(node, node.pos_);
            cur->nxt_ = p;
            cur = p;
        }
    }
    // std::cerr << "end\n";
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
BlockList<Tkey, Tvalue, max_size, block_size>::~BlockList() {
    headmemory_.write_info(size_, 2);
    headmemory_.write_info(head_->pos_, 3);
    int t;
    headmemory_.get_info(t, 2);
    // std::cerr << "test " << size_ << " " << t << " " << head_->pos_ << "\n";
    while (1) {
        headmemory_.update(head_->node_, head_->pos_);
        auto tmp = head_->nxt_;
        delete head_;
        if (!tmp) {
            break;
        }
        head_ = tmp;
    }
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockInsert
(HeadList *cur, const std::pair<Tkey, Tvalue> &v) {
    BlockNode vec;
    nodememory_.read(vec, cur->pos_);
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
        auto p = new HeadList({R[0], R.back(), R.size(), cur->node_.nxt_, pos}, pos);
        nodememory_.update(L, cur->pos_);
        auto tmp = cur->nxt_;
        cur->nxt_ = p;
        cur->node_.nxt_ = pos;
        p->nxt_ = tmp;
    } else {
        nodememory_.update(vec, cur->pos_);
        cur->node_ = {vec[0], vec.back(), vec.size(), cur->node_.nxt_, cur->node_.pos_};
    }
}
template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockModify
(HeadList *cur, const MyVector<std::pair<Tkey, Tvalue>, block_size * 2> &v) {
    BlockNode vec;
    nodememory_.read(vec, cur->pos_);
    vec = v;
    nodememory_.update(vec, cur->pos_);
    cur->node_ = {vec[0], vec.back(), vec.size(), cur->node_.nxt_, cur->node_.pos_};
}
template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockDelete
(HeadList *cur, const std::pair<Tkey, Tvalue> &v) {
    MyVector<std::pair<Tkey, Tvalue>, block_size * 2> vec;
    nodememory_.read(vec, cur->pos_);
    auto p = vec.lower_bound(v);
    // std::cerr << "DELETE: " << p << " " << vec.size() << " " << (vec[p] == v) << "\n";
    if (p == size_ || vec[p] != v) {
        return;
    }
    vec.erase(p);
    nodememory_.update(vec, cur->pos_);
    cur->node_ = {vec[0], vec.back(), vec.size(), cur->node_.nxt_, cur->node_.pos_};
}
template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
std::vector<Tvalue> BlockList<Tkey, Tvalue, max_size, block_size>::BlockFind
(HeadList *cur, const Tkey &key) {
    BlockNode vec;
    // std::cerr << cur->pos_ << " " << sizeof(BlockNode) << "\n";
    nodememory_.read(vec, cur->pos_);
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
void BlockList<Tkey, Tvalue, max_size, block_size>::Insert
(const std::pair<Tkey, Tvalue> &v) {
    if (size_ == 0) {
        auto p = BlockNode(v);
        auto pos = nodememory_.write(p);
        HeadNode t = {v, v, 1, 0, pos};
        head_ = new HeadList(t, pos);
    } else {
        auto p = head_;
        while (1) {
            if (!p->nxt_) {
                BlockInsert(p, v);
                break;
            }
            if (v < p->nxt_->node_.head_) {
                BlockInsert(p, v);
                break;
            }
            p = p->nxt_;
        }
    }
    ++size_;
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::Delete
(const std::pair<Tkey, Tvalue> &v) {
    if (size_ == 0) {
        return;
    } 
    auto p = head_;
    while (1) {
        if (p->node_.head_ <= v && v <= p->node_.end_) {
            BlockDelete(p, v);
            break;
        }
        if (!p->nxt_ || p->nxt_->node_.head_ > v) {
            break;
        }
        p = p->nxt_;
    }
}

template <class Tkey, class Tvalue, size_t max_size, size_t block_size>
std::vector<Tvalue> BlockList<Tkey, Tvalue, max_size, block_size>::Find(const Tkey &key) {
    if (size_ == 0) {
        return {};
    } 
    auto p = head_;
    std::vector<Tvalue> res;
    while (1) {
        // std::cerr << "etst\n";
        // std::cerr << p->node_.head_.first << " " << p->node_.end_.first << " " << p->nxt_ << "\n";
        if (p->node_.head_.first <= key && key <= p->node_.end_.first) {
            auto vec = BlockFind(p, key);
            for (size_t i = 0; i < vec.size(); i++) {
                res.push_back(vec[i]);
            }
        }
        // std::cerr << "end\n";
        if (!p->nxt_ || p->nxt_->node_.head_.first > key) {
            break;
        }
        p = p->nxt_;
    }
    return res;
}

#endif // BLOCKLIST_HPP