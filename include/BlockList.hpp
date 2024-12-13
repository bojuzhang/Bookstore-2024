#ifndef BLOCKLIST_HPP
#define BLOCKLIST_HPP

#include "MemoryRiver.hpp"
#include <cstddef>
#include <utility>
#include <vector>

template <typename Tkey, typename Tvalue, size_t max_size = 100000, size_t block_size = 350>
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
    using BlockNode = std::vector<std::pair<Tkey, Tvalue>>;
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
    void BlockModify(HeadList *, const std::vector<std::pair<Tkey, Tvalue>> &);
    std::vector<Tvalue> BlockFind(HeadList *, const Tkey &);

    void Insert(const std::pair<Tkey, Tvalue> &);
    void Delete(const std::pair<Tkey, Tvalue> &);
    std::vector<Tvalue> Find(const Tkey &);
    size_t size() {return size_;}
};

#endif // BLOCKLIST_HPP