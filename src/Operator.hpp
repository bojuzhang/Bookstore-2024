#pragma once
#include "Account.hpp"
#include "Book.hpp"
#include "Log.hpp"
#include "MyVector.hpp"
#include <iostream>
#include <vector>
#ifndef OPERATOR_HPP
#define OPERATOR_HPP

enum class ShowOperator {ISBN, BOOKNAME, AUTHOR, KEYWORD, ALL};
enum class ModifyOperator {ISBN, BOOKNAME, AUTHOR, KEYWORD, PRICE};

class Operator {
private:
    BookSystem booksystem_;
    AccountSystem accountsystem_;
    LogSystem logsystem_;
    bool CheckPriviledge(int priviledge) {
        if (accountsystem_.NowPriviledge() < priviledge) {
            std::cout << "Invalid\n";
            return false;
        }
        return true;
    }
public:
    void Login(const string30 &, const string30 &);
    void Logout();
    void Register(const string30 &, const string30 &, const string30 &);
    void ModifyPassword(const string30 &, const string30 &, const string30 &);
    void AddUser(const string30 &, const string30 &, int, const string30 &);
    void DeleteUser(const string30 &);

    void Show(ShowOperator, const string60 &, const string20 &);
    void BuyBook(const string20 &, int);
    void Select(const string20 &);
    void Modify(const std::vector<ModifyOperator> &, const std::vector<string60> &, const string20 &);
    void Import(int, double);

    void ShowFinance(int, bool);
    // TODO: WHOLE LOG
    void Log();
    void ReportFinance();
    void ReportEmployee();
};

inline void Operator::Login(const string30 &userid, const string30 &password) {
    if (!CheckPriviledge(0)) return;
    if (!accountsystem_.CheckExist(userid)) {
        std::cout << "Invalid\n";
        return;
    }
    Account user = accountsystem_.FindAccount(userid);
    if (accountsystem_.NowPriviledge() > user.priviledge) {
        
    }
}
inline void Operator::Logout() {

}
inline void Operator::Register(const string30 &, const string30 &, const string30 &) {

}
inline void Operator::ModifyPassword(const string30 &, const string30 &, const string30 &) {

}
inline void Operator::AddUser(const string30 &, const string30 &, int, const string30 &) {

}
inline void Operator::DeleteUser(const string30 &) {

}

inline void Operator::Show(ShowOperator, const string60 &, const string20 &) {

}
inline void Operator::BuyBook(const string20 &, int) {

}
inline void Operator::Select(const string20 &) {

}
inline void Operator::Modify(const std::vector<ModifyOperator> &, const std::vector<string60> &, const string20 &) {

}
inline void Operator::Import(int, double) {

}

inline void Operator::ShowFinance(int, bool) {

}

#endif // OPERATOR_HPP