#include "BlockList.hpp"
#include "MyVector.hpp"
#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
BlockList<Tkey, Tvalue, max_size, block_size>::BlockList(const std::string &file_name) {
    headmemory_.initialise(file_name + "_head");
    nodememory_.initialise(file_name + "_node");
    
    headmemory_.get_info(size_, 1);

    if (size_) {
        size_t head_pos;
        headmemory_.get_info(head_pos, 2);
        HeadNode node;
        headmemory_.read(node, head_pos);
        head_(node, node.pos_);
        auto cur = head_;
        while (node.nxt_) {
            headmemory_.read(node, node.nxt_);
            auto p = new HeadList(node, node.pos_);
            cur->nxt_ = p;
            cur = p;
        }
    }
}

template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
BlockList<Tkey, Tvalue, max_size, block_size>::~BlockList() {
    headmemory_.write_info(size_, 1);
    headmemory_.write_info(head_->pos_, 2);
    while (1) {
        headmemory_.update(head_->pos_, head_->node_);
        auto tmp = head_->nxt_;
        delete head_;
        if (!tmp) {
            break;
        }
        head_ = tmp;
    }
}

template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
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
template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockModify
(HeadList *cur, const MyVector<std::pair<Tkey, Tvalue>, block_size * 2> &v) {
    BlockNode vec;
    nodememory_.read(vec, cur->pos_);
    vec = v;
    nodememory_.update(vec, cur->pos_);
    cur->node_ = {vec[0], vec.back(), vec.size(), cur->node_.nxt_, cur->node_.pos_};
}
template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::BlockDelete
(HeadList *cur, const std::pair<Tkey, Tvalue> &v) {
    BlockNode vec;
    nodememory_.read(vec, cur->pos_);
    auto p = std::lower_bound(vec.begin(), vec.end(), v);
    if (p != vec.end() && (*p) == v) {
        return;
    }
    vec.erase(p);
    nodememory_.update(vec, cur->pos_);
    cur->node_ = {vec[0], vec.back(), vec.size(), cur->node_.nxt_, cur->node_.pos_};
}
template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
std::vector<Tvalue> BlockList<Tkey, Tvalue, max_size, block_size>::BlockFind
(HeadList *cur, const Tkey &key) {
    BlockNode vec;
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

template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
void BlockList<Tkey, Tvalue, max_size, block_size>::Insert
(const std::pair<Tkey, Tvalue> &v) {
    if (size_ == 0) {
        auto pos = nodememory_.write(BlockNode{v});
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

template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
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

template <typename Tkey, typename Tvalue, size_t max_size, size_t block_size>
std::vector<Tvalue> BlockList<Tkey, Tvalue, max_size, block_size>::Find(const Tkey &key) {
    if (size_ == 0) {
        return {};
    } 
    auto p = head_;
    std::vector<Tvalue> res;
    while (1) {
        if (p->node_.head_.first <= key && key <= p->node_.end_) {
            auto vec = BlockFind(p, key);
            for (size_t i = 0; i < vec.size(); i++) {
                res.push_back(vec[i]);
            }
        }
        if (!p->nxt_ || p->nxt_->node_.head_.first > key) {
            break;
        }
        p = p->nxt_;
    }
    return res;
}