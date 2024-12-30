#pragma once
#include "Book.hpp"
#include "Log.hpp"
#include <cstdint>
#ifndef FORMAT_HPP
#define FORMAT_HPP
#include <cstddef>
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
    bool CheckUserInfo(const string &); 
    bool CheckBookInfo(const string &);
    bool CheckAll(const string &);

    void Login();
    void Logout();
    void Register();
    void ModifyPassword();
    void AddUser();
    void DeleteUser();
    void LogoutAll();

    void Show();// Show Book & Show Finance
    void BuyBook();
    void Select();
    void Modify();
    void Import();

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
    if (ans[1].size() && ans[1][0] == '\"' && ans[1].back() == '\"') {
        ans[1].erase(0, 1);
        ans[1].pop_back();
    }
    return ans;
}

inline bool Format::CheckUserInfo(const string &s) {
    for (auto c : s) {
        if (!std::isdigit(c) && !isalpha(c) && c != '_') {
            return false;
        }
    }
    return true;
} 
inline bool Format::CheckBookInfo(const string &s) {
    return s.find('\"') == s.npos;
}
inline bool Format::CheckAll(const string &s) {
    for (auto c : s) {
        if (isspace(c) || c < 32 || c > 126 || !std::isprint(c)) {
            return false;
        }
    }
    return true;
}

inline void Format::LogoutAll() {
    operator_.LogoutAll();
}

inline void Format::run() {
    std::string op;
    while (std::cin >> op) {
        if (op == "quit" || op == "exit") {
            auto p = GetToken();
            if (p.empty()) {
                LogoutAll();
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
        } else if (op == "buy") {
            BuyBook();
        } else if (op == "select") {
            Select();
        } else if (op == "modify") {
            Modify();
        } else if (op == "import") {
            Import();
        } else if (op == "log") {
            auto p = GetToken();
            if (p.size()) {
                std::cout << "Invalid\n";
                continue;
            }
            Log();
        } else if (op == "report") {
            auto p = GetToken();
            if (p.size() != 1) {
                std::cout << "Invalid\n";
                continue;
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
    operator_.LogoutAll();
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
        if (s.size() > 30 || !CheckUserInfo(s) || !CheckAll(s)) {
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
        if (s.size() > 30 || !CheckAll(s)) {
            std::cout << "Invalid\n";
            return;
        }
    }
    if (!CheckUserInfo(p[0]) || !CheckUserInfo(p[1])) {
        std::cout << "Invalid\n";
        return;
    }
    std::cerr << "success register";
    operator_.Register(string30(p[0]), string30(p[1]), string30(p[2]));
}
inline void Format::ModifyPassword() {
    auto p = GetToken();
    if (p.size() != 2 && p.size() != 3) {
        std::cout << "Invalid\n";
        return;
    }
    for (const auto &s : p) {
        if (s.size() > 30 || !CheckUserInfo(s) || !CheckAll(s)) {
            std::cout << "Invalid\n";
            return;
        }
    }
    operator_.ModifyPassword(string30(p[0]), string30(p.size() == 3 ? p[1] : ""), string30(p.back()));
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
    for (auto id : {0,1}) {
        if (!CheckUserInfo(p[id]) || !CheckAll(p[id])) {
            std::cout << "Invalid\n";
            return;
        }
    }
    if (!CheckAll(p[3])) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[2] != "1" && p[2] != "3" && p[2] != "7") {
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
    if (p[0].size() > 30 || !CheckUserInfo(p[0]) || !CheckAll(p[0])) {
        std::cout << "Invalid\n";
        return;
    }
    operator_.DeleteUser(string30(p[0]));
}

// Show book & Show finance
inline void Format::Show() {
    auto p = GetToken();
    if (p.size() > 2) {
        std::cout << "Invalid\n";
        return;
    }
    if (p.size() == 0) {
        operator_.Show(ShowOperator::ALL, string60(""), string20(""));
    } else if (p.size() == 1) {
        if (p[0] == "finance") {
            operator_.ShowFinance(1, 1);
            return;
        }
        auto t = ReleaseToken(p[0]);
        auto op = t[0], str = t[1];
        if (op == "ISBN") {
            if (str.size() > 20 || str.empty() || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            operator_.Show(ShowOperator::ISBN, string60(""), string20(str));
        } else if (op == "name") {
            if (str.size() > 60 || str.empty() || !CheckBookInfo(str) || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            operator_.Show(ShowOperator::BOOKNAME, string60(str), string20(""));
        } else if (op == "author") {
            if (str.size() > 60 || str.empty() || !CheckBookInfo(str) || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            operator_.Show(ShowOperator::AUTHOR, string60(str), string20(""));
        } else if (op == "keyword") {
            std::cerr << "testkeyword " << str << " " << str.find('|') << " " << str.npos << "\n";
            if (str.size() > 60 || str.empty() || str.find('|') != str.npos || !CheckBookInfo(str) || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            auto ve = ReleaseKeywords(str);
            if (ve.size() > 1) {
                std::cout << "Invalid\n";
                return;
            }
            for (const auto &p : ve) {
                if (p.empty()) {
                    std::cout << "Invalid\n";
                    return;
                }
            }
            operator_.Show(ShowOperator::KEYWORD, string60(str), string20(""));
        } else {
            std::cout << "Invalid\n";
            return;
        }
    } else {
        if (p[0] != "finance") {
            std::cout << "Invalid\n";
            return;
        }
        if (p[1].size() > 10) {
            std::cout << "Invalid\n";
            return;
        }
        for (auto c : p[1]) {
            if (!isdigit(c)) {
                std::cout << "Invalid\n";
                return;
            }
        }
        if (std::stoll(p[1]) >= INT32_MAX) {
            std::cout << "Invalid\n";
            return;
        }
        operator_.ShowFinance(std::stoi(p[1]), 0);
    }
}
inline void Format::BuyBook() {
    auto p =GetToken();
    std::cerr << "test buy book\n";
    if (p.size() != 2) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[0].size() > 20 || p[1].size() > 10) {
        std::cout << "Invalid\n";
        return;
    }
    if (!CheckBookInfo(p[0]) || !CheckAll(p[1])) {
        std::cout << "Invalid\n";
        return;
    }
    for (auto c : p[1]) {
        if (!isdigit(c)) {
            std::cout << "Invalid\n";
            return;
        }
    }
    int num = std::stoi(p[1]);
    if (num <= 0) {
        std::cout << "Invalid\n";
        return;
    }
    std::cerr << "buy book token: " << p[0] << " " << p[1] << "\n";
    operator_.BuyBook(string20(p[0]), std::stoi(p[1]));
}
inline void Format::Select() {
    auto p =GetToken();
    if (p.size() != 1) {
        std::cout << "Invalid\n";
        return;
    }
    if (p[0].size() > 20 || !CheckAll(p[0])) {
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
            if (str.size() > 20 || str.empty() || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::ISBN);
            isbn = string20(str);
        } else if (op == "name") {
            if (str.size() > 60 || str.empty() || !CheckBookInfo(str) || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::BOOKNAME);
            others.push_back(string60(str));
        } else if (op == "author") {
            if (str.size() > 60 || str.empty() || !CheckBookInfo(str) || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::AUTHOR);
            others.push_back(string60(str));
        } else if (op == "keyword") {
            if (str.size() > 60 || str.empty() || !CheckBookInfo(str) || !CheckAll(str)) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::KEYWORD);
            auto ve = ReleaseKeywords(str);
            sort(ve.begin(), ve.end());
            for (size_t i = 1; i < ve.size(); i++) {
                if (ve[i - 1] == ve[i]) {
                    std::cout << "Invalid\n";
                    return;
                }
            }
            for (const auto &p : ve) {
                if (p.empty()) {
                    std::cout << "Invalid\n";
                    return;
                }
            }
            others.push_back(string60(str));
        } else if (op == "price") {
            if (str.size() > 13 || str.empty()) {
                std::cout << "Invalid\n";
                return;
            }
            ops.push_back(ModifyOperator::PRICE);
            int cnt_of_dot = 0, cnt_res = 0;
            for (auto c : str) {
                if (!isdigit(c) && c != '.') {
                    std::cout << "Invalid\n";
                    return;
                }
                if (cnt_of_dot) ++cnt_res;
                if (c == '.') ++cnt_of_dot;
            }
            if (cnt_of_dot >= 2 || cnt_res > 2) {
                std::cout << "Invalid\n";
                return;
            }
            price = std::stod(str);
            if (price <= 0) {
                std::cout << "Invalid\n";
                return;
            }
        } else {
            std::cout << "Invalid\n";
            return;
        }
    }
    for (size_t i = 0; i < ops.size(); i++) {
        for (size_t j = i + 1; j < ops.size(); j++) {
            if (ops[i] == ops[j]) {
                std::cout << "Invalid\n";
                return;
            }
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
    int cnt_of_dot = 0, cnt_res = 0;
    for (auto c : p[1]) {
        if (!isdigit(c) && c != '.') {
            std::cout << "Invalid\n";
            return;
        }
        if (cnt_of_dot) ++cnt_res;
        if (c == '.') ++cnt_of_dot;
    }
    if (cnt_of_dot >= 2 || cnt_res > 2) {
        std::cout << "Invalid\n";
        return;
    }
    if (std::stoll(p[0]) >= INT32_MAX || std::stoll(p[0]) <= 0 || std::stod(p[1]) <= 0) {
        std::cout << "Invalid\n";
        return;
    }
    operator_.Import(std::stoi(p[0]), std::stod(p[1]));
}

// TODO: WHOLE LOG
inline void Format::Log() {
    operator_.Log();
}
inline void Format::ReportFinance() {
    operator_.ReportFinance();
}
inline void Format::ReportEmployee() {
    operator_.ReportEmployee();
}

#endif //FORMAT_HPP