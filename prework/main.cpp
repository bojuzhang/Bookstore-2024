#include "BlockList.hpp"
#include "MyVector.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

BlockList<MyString<64>, int> T("testBlockList");

int main(int argc, const char *argv[]) {
    int n;
    std::cin >> n;
    while (n--) {
        std::string op, index;
        int value;
        std::cin >> op >> index;
        MyString<64> s(index);
        if (op == "find") {
            auto ans = T.Find(s);
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
            auto p = std::make_pair(s, value);
            T.Insert(p);
        } else {
            std::cin >> value;
            T.Delete(std::make_pair(s, value));
        }
    }
}