#pragma once
#ifndef FORMAT_HPP
#define FORMAT_HPP
#include "Operator.hpp"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

class Format {
private:
    Operator operator_;
    std::vector<std::string> GetToken();

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

}
inline void Format::Logout() {

}
inline void Format::Register() {

}
inline void Format::ModifyPassword() {

}
inline void Format::AddUser() {

}
inline void Format::DeleteUser() {

}

inline void Format::Show() {

}
inline void Format::BuyBook() {

}
inline void Format::Select() {

}
inline void Format::Modify() {

}
inline void Format::Import() {

}

inline void Format::ShowFinance() {

}
// TODO: WHOLE LOG
inline void Format::Log() {

}
inline void Format::ReportFinance() {

}
inline void Format::ReportEmployee() {

}

#endif //FORMAT_HPP