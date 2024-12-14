#include "BlockList.hpp"
#include "MemoryRiver.hpp"
#include "MyHash.hpp"
#include "MyVector.hpp"
#include <iostream>
#include <string>
#include <utility>

BlockList<int, int> T("testBlockList");

int main(int argc, const char *argv[]) {
    int n;
    std::cin >> n;
    while (n--) {
        std::string op, index;
        int value;
        std::cin >> op >> index;
        if (op == "find") {
            auto ans = T.Find(GetHash(index));
            if (ans.empty()) {
                std::cout << "null\n";
            } else {
                for (const auto &x : ans) {
                    std::cout << x << " ";
                }
                std::cout << "\n";
            }
        } else if (op == "insert") {
            std::cin >> value;
            auto p = std::make_pair(GetHash(index), value);
            T.Insert(p);
        } else {
            std::cin >> value;
            T.Delete(std::make_pair(GetHash(index), value));
        }
    }

    // using node = MyVector<int, 100>;
    // MemoryRiver<node> a("123456");
    // a.initialise();
    // std::cerr << "fdsfhjsd: " << sizeof(node) << "\n";
    // node ve(std::vector<int>(5, GetHash("114!!!")));
    // a.write(ve);
    // ve.print();
    // node p;
    // a.read(p, 0);
    // p.print();
}