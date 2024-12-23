#pragma once
#include <cassert>
#include <cstddef>
#ifndef FORMAT_HPP
#define FORMAT_HPP
#include <array>
#include "MyVector.hpp"
#include "Operator.hpp"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

class Format {
private:
    Operator operator_;
    std::vector<std::string> GetToken();
    std::array<std::string, 2> ReleaseToken(std::string);

    void Login();
    void Logout();
    void Register();
    void ModifyPassword();
    void AddUser();
    void DeleteUser();

    void Show();
    void BuyBook();
    void Select();
    void Modify();
    void Import();

    void ShowFinance();
    // TODO: WHOLE LOG
    void Log();
    void ReportFinance();
    void ReportEmployee();
public:
    void run();
};

inline std::vector<std::string> Format::GetToken() {
    std::vector<std::string> v;
    std::string s, t;
    std::getline(std::cin, s);
    for (auto c : s) {
        if (isspace(c)) {
            if (!t.empty()) {
                v.push_back(t);
                t = "";
            }
            continue;
        }
        t += c;
    }
    if (!t.empty()) {
        v.push_back(t);
        t = "";
    }
    return v;
}

inline std::array<std::string, 2> Format::ReleaseToken(std::string s) {
    std::array<std::string, 2> ans;
    if (s[0] != '-') {
        return {"Invalid", "Invalid"};
    }
    size_t i = 1;
    while (i < s.size() && s[i] != '=') {
        ans[0] += s[i];
        i++;
    }
    i++;
    while (i < s.size()) {
        ans[1] += s[i];
        i++;
    }
    return ans;
}

inline void Format::run() {
    while (1) {
        std::string op;
        std::cin >> op;
        if (op == "quit" || op == "exit") {
            auto p = GetToken();
            if (p.empty()) {
                break; 
            } else {
                std::cout << "Invalid\n";
                continue;
            }
        } else if (op == "su") {
            Login();
        } else if (op == "logout") {
            Logout();
        } else if (op == "register") {
            Register();
        } else if (op == "passwd") {
            ModifyPassword();
        } else if (op == "useradd") {
            AddUser();
        } else if (op == "delete") {
            DeleteUser();
        } else if (op == "show") {
            Show();
        } else if (op == "log") {
            Log();
        } else if (op == "report") {
            auto p = GetToken();
            if (p.size() != 1) {
                std::cout << "Invalid\n";
                return;
            }
            if (p[0] == "finance") {
                ReportFinance();
            } else if (p[0] == "employee") {
                ReportEmployee();
            } else {
                std::cout << "Invalid\n";
                continue;
            }
        } else {
            GetToken();
            std::cout << "Invalid\n";
        }
    }
}

inline void Format::Login() {
    auto p = GetToken();
    if (p.size() == 0 || p.size() > 2) {
        std::cout << "Invalid\n";
        return;
    }
    if (p.size() == 1) {
        p.push_back("");
    }
    for (const auto &s : p) {
        if (s.size() > 30) {
            std::cout << "Invalid\n";
            return;
        }
    }
    operator_.Login(string30(p[0]), string30(p[1]));
}
inline void Format::Logout() {
    auto p = GetToken();
    if (p.size()) {
        std::cout << "Invalid\n";
        return;
    }
    operator_.Logout();
}
inline void Format::Register() {
    auto p = GetToken();
    if (p.size() != 3) {
        std::cout << "Invalid\n";
        return;
    }
    for (const auto &s : p) {
        if (s.size() > 30) {
            std::cout << "Invalid\n";
            return;
        }
    }
    operator_.Register(string30(p[0]), string30(p[1]), string30(p[2]));
}
inline void Format::ModifyPassword() {
    auto p = GetToken();
    if (p.size() != 2 && p.size() != 3) {
        std::cout << "Invalid\n";
        return;
    }
    for (const auto &s : p) {
        if (s.size() > 30) {
            std::cout << "Invalid\n";
            return;
        }
    }
    operator_.ModifyPassword(string30(p[0]), string30(p.size() == 3 ? p[2] : ""), string30(p.back()));
}
inline void Format::AddUser() {
    auto p = GetToken();
    if (p.size() != 4) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[0].size() > 30 || p[1].size() > 30 || p[3].size() > 30 || p[2].size() > 1) {
        std::cout << "Invalid\n";
        return;
    }
    operator_.AddUser(string30(p[0]), string30(p[1]), std::stoi(p[2]), string30(p[3]));
}
inline void Format::DeleteUser() {
    auto p = GetToken();
    if (p.size() != 1) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[0].size() > 30) {
        std::cout << "Invalid\n";
        return;
    }
    operator_.DeleteUser(string30(p[0]));
}

inline void Format::Show() {
    auto p = GetToken();
    if (p.size() > 1) {
        std::cout << "Invalid\n";
        return;
    }
    if (p.size() == 0) {
        operator_.Show(ShowOperator::ALL, string60(""), string20(""));
    } else {
        auto t = ReleaseToken(p[0]);
        auto op = t[0], str = t[1];
        if (op == "ISBN") {
            if (str.size() > 20 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            operator_.Show(ShowOperator::ISBN, string60(""), string20(str));
        } else if (op == "name") {
            if (str.size() > 60 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            operator_.Show(ShowOperator::BOOKNAME, string60(p[0]), string20(""));
        } else if (op == "author") {
            if (str.size() > 60 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            operator_.Show(ShowOperator::AUTHOR, string60(p[0]), string20(""));
        } else if (op == "keyword") {
            if (str.size() > 60 || str.empty() || str.find('|') != str.npos) {
                std::cout << "Invalid\n";
                return;
            }
            operator_.Show(ShowOperator::KEYWORD, string60(p[0]), string20(""));
        } else {
            std::cout << "Invalid\n";
            return;
        }
    }
}
inline void Format::BuyBook() {
    auto p =GetToken();
    if (p.size() != 2) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[0].size() > 20 || p[1].size() > 10) {
        std::cout << "Invalid\n";
        return;
    }
    for (auto c : p[1]) {
        if (!isdigit(c)) {
            std::cout << "Invalid\n";
            return;
        }
    }
    operator_.BuyBook(string20(p[0]), std::stoi(p[1]));
}
inline void Format::Select() {
    auto p =GetToken();
    if (p.size() != 1) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[0].size() > 20) {
        std::cout << "Invalid\n";
        return;
    }
    operator_.Select(string20(p[0]));
}
inline void Format::Modify() {
    auto p = GetToken();
    if (!p.size() || p.size() > 5) {
        std::cout << "Invalid\n";
        return;
    }
    string20 isbn("");
    double price = 0.0;
    std::vector<ModifyOperator> ops;
    std::vector<string60> others;
    for (const auto &x : p) {
        auto t = ReleaseToken(x);
        auto op = t[0], str = t[1];
        if (op == "ISBN") {
            if (str.size() > 20 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::ISBN);
            isbn = string20(str);
        } else if (op == "name") {
            if (str.size() > 60 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::BOOKNAME);
            others.push_back(string60(str));
        } else if (op == "author") {
            if (str.size() > 60 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::AUTHOR);
            others.push_back(string60(str));
        } else if (op == "keyword") {
            if (str.size() > 60 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::KEYWORD);
            others.push_back(string60(str));
        } else if (op == "price") {
            if (str.size() > 13 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::PRICE);
            for (auto c : str) {
                if (!isdigit(c) && c != '.') {
                    std::cout << "Invalid\n";
                    return;
                }
            }
            price = std::stod(str);
        } else {
            std::cout << "Invalid\n";
            return;
        }
    }
    operator_.Modify(ops, others, isbn, price);
}
inline void Format::Import() {
    auto p = GetToken();
    if (p.size() != 2) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[0].size() > 10 || p[1].size() > 13) {
        std::cout << "Invalid\n";
        return;
    }
    for (auto c : p[0]) {
        if (!isdigit(c)) {
            std::cout << "Invalid\n";
            return;
        }
    }
    for (auto c : p[1]) {
        if (!isdigit(c) && c != '.') {
            std::cout << "Invalid\n";
            return;
        }
    }
    operator_.Import(std::stoi(p[0]), std::stod(p[1]));
}

inline void Format::ShowFinance() {
    auto p = GetToken();
    if (p.size() > 1) {
        std::cout << "Invalid\n";
        return;
    }
    if (p.empty()) {
        operator_.ShowFinance(1, 1);
    } else {
        if (p[0].size() > 10) {
            std::cout << "Invalid\n";
            return;
        }
        for (auto c : p[0]) {
            if (!isdigit(c)) {
                std::cout << "Invalid\n";
                return;
            }
        }
        operator_.ShowFinance(std::stoi(p[0]), 0);
    }
}
// TODO: WHOLE LOG
inline void Format::Log() {

}
inline void Format::ReportFinance() {

}
inline void Format::ReportEmployee() {

}

#endif //FORMAT_HPP