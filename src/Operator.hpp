#pragma once
#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <ios>
#include <string>
#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include "Account.hpp"
#include "Book.hpp"
#include "Log.hpp"
#include "MyVector.hpp"
#include <iostream>
#include <vector>

enum class ShowOperator {ISBN, BOOKNAME, AUTHOR, KEYWORD, ALL};
enum class ModifyOperator {ISBN, BOOKNAME, AUTHOR, KEYWORD, PRICE};

class Operator {
private:
    BookSystem booksystem_;
    AccountSystem accountsystem_;
    LogSystem logsystem_;
    bool CheckPriviledge(int priviledge) {
        if (accountsystem_.NowPriviledge() < priviledge) {
            // std::cout << "Invalid\n";
            return false;
        }
        return true;
    }
public:
    void Login(const string30 &, const string30 &);
    void Logout();
    void LogoutAll();
    void Register(const string30 &, const string30 &, const string30 &);
    void ModifyPassword(const string30 &, const string30 &, const string30 &);
    void AddUser(const string30 &, const string30 &, int, const string30 &);
    void DeleteUser(const string30 &);

    void Show(ShowOperator, const string60 &, const string20 &);
    void BuyBook(const string20 &, int);
    void Select(const string20 &);
    void Modify(const std::vector<ModifyOperator> &, const std::vector<string60> &, const string20 &, double);
    void Import(int, double);

    void ShowFinance(int, bool);
    // TODO: WHOLE LOG
    void Log();
    void ReportFinance();
    void ReportEmployee();
};

inline void Operator::Login(const string30 &userid, const string30 &password) {
    if (!CheckPriviledge(0))  {
        std::cout << "Invalid\n";
        return;
    }
    if (!accountsystem_.CheckExist(userid)) {
        std::cerr << "nouser\n";
        std::cout << "Invalid\n";
        return;
    }
    Account user = accountsystem_.FindAccount(userid);
    if (password.empty() && accountsystem_.NowPriviledge() <= user.priviledge) {
        std::cerr << "nopasswordpriviledge\n";
        std::cout << "Invalid\n";
        return;
    }
    if (!password.empty() && user.password != password) {
        std::cerr << "worngpassword\n";
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.Login(userid);
}
inline void Operator::Logout() {
    if (!CheckPriviledge(1)) {
        std::cout << "Invalid\n";
        return;
    }
    if (accountsystem_.size() <= 0) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.Logout();
}
inline void Operator::LogoutAll() {
    while (accountsystem_.size()) {
        accountsystem_.Logout();
    }
}
inline void Operator::Register(const string30 &userid, const string30 &password, const string30 &username) {
    if (!CheckPriviledge(0)) {
        std::cout << "Invalid\n";
        return;
    }
    if (accountsystem_.CheckExist(userid)) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.AddAccount(userid, username, password, 1);
}
inline void Operator::ModifyPassword
(const string30 &userid, const string30 &currentpassword, const string30 &newpassword) {
    if (!CheckPriviledge(1)) {
        std::cout << "Invalid\n";
        return;
    }
    if (!accountsystem_.CheckExist(userid)) {
        std::cout << "Invalid\n";
        return;
    }
    auto user = accountsystem_.FindAccount(userid);
    if (currentpassword.empty() && accountsystem_.NowPriviledge() < 7) {
        std::cout << "Invalid\n";
        return;
    }
    if (!currentpassword.empty() && currentpassword != user.password) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.ModifyPassword(userid, newpassword);
    logsystem_.AddLog(accountsystem_.Nowid(), logstring("passwd " + userid.str() + " " + currentpassword.str() + " " + newpassword.str()));
}
inline void Operator::AddUser
(const string30 &userid, const string30 &password, int priviledge, const string30 &username) {
    if (!CheckPriviledge(3)) {
        std::cerr << "adduserpriviledge\n";
        std::cout << "Invalid\n";
        return;
    }
    if (priviledge >= accountsystem_.NowPriviledge()) {
        std::cerr << "adduserpriviledgelow\n";
        std::cout << "Invalid\n";
        return;
    }
    if (accountsystem_.CheckExist(userid)) {
        std::cerr << "adduersexist\n";
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.AddAccount(userid, username, password, priviledge);
    logsystem_.AddLog(accountsystem_.Nowid(), logstring("adduser " + userid.str() + " " + username.str() + " " + password.str() + " " + std::to_string(priviledge)));
}
inline void Operator::DeleteUser(const string30 &userid) {
    if (!CheckPriviledge(7)) {
        std::cerr << "delpri\n";
        std::cout << "Invalid\n";
        return;
    }
    if (!accountsystem_.CheckExist(userid)) {
        std::cerr << "delnoexist\n";
        std::cout << "Invalid\n";
        return;
    }
    auto user = accountsystem_.FindAccount(userid);
    // std::cerr << user.online_cnt << "\n";
    if (user.online_cnt > 0) {
        std::cerr << "delonline\n";
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.DeleteAccount(user);
    logsystem_.AddLog(accountsystem_.Nowid(), logstring("delete " + userid.str()));
}

inline void Operator::Show(ShowOperator op, const string60 &other, const string20 &isbn) {
    if (!CheckPriviledge(1)) {
        std::cout << "Invalid\n";
        return;
    }
    std::string res = "show ";
    if (op == ShowOperator::ISBN) {
        booksystem_.ShowIsbn(isbn);
        res += "-isbn=" + isbn.str() + " ";
    } else if (op == ShowOperator::AUTHOR) {
        booksystem_.ShowAuthor(other);
        res += "-author=" + other.str() + " ";
    } else if (op == ShowOperator::BOOKNAME) {
        booksystem_.ShowName(other);
        res += "-name=" + other.str() + " ";
    } else if (op == ShowOperator::KEYWORD) {
        booksystem_.ShowKeyword(other);
        res += "-keyword=" + other.str() + " ";
    } else if (op == ShowOperator::ALL) {
        booksystem_.ShowAll();
        res += "-all";
    } else {
        std::cout << "Invalid\n";
        return;
    }
    logsystem_.AddLog(accountsystem_.Nowid(), logstring(res));
}
inline void Operator::BuyBook(const string20 &isbn, int num) {
    if (!CheckPriviledge(1)) {
        std::cerr << "not enough priviledge\n";
        std::cout << "Invalid\n";
        return;
    }
    if (!booksystem_.CheckExist(isbn)) {
        std::cerr << "book no exist\n";
        std::cout << "Invalid\n";
        return;
    }
    int id = booksystem_.FindId(isbn);
    auto book = booksystem_.FindBook(id);
    if (book.rem < num) {
        std::cerr << "book rem invalid " << isbn << " " << book.rem << "\n";
        std::cout << "Invalid\n";
        return;
    }
    booksystem_.BuyBook(id, num);
    logsystem_.AddFinance(book.price * num, 1);
    std::cerr << "buy book success " << isbn << " " << num << " " << book.rem << " " << booksystem_.FindBook(id).rem << "\n";
    std::cout << std::fixed << std::setprecision(2) << book.price * num << "\n";
    logsystem_.AddLog(accountsystem_.Nowid(), logstring("buy " + isbn.str() + " " + std::to_string(num)));
}
inline void Operator::Select(const string20 &isbn) {
    if (!CheckPriviledge(3)) {
        std::cerr << "select priviledge invalid\n";
        std::cout << "Invalid\n";
        return;
    }
    if (!booksystem_.CheckExist(isbn)) {
        booksystem_.AddBook(isbn);
    }
    int id = booksystem_.FindId(isbn);
    accountsystem_.Select(id);
    logsystem_.AddLog(accountsystem_.Nowid(), logstring("select " + isbn.str()));
}
inline void Operator::Modify
(const std::vector<ModifyOperator> &ops, const std::vector<string60> &others, 
 const string20 &isbn, double price) {
    if (!CheckPriviledge(3)) {
        std::cerr << "modify priviledge invalid\n";
        std::cout << "Invalid\n";
        return;
    }
    int id = accountsystem_.NowSelect();
    if (id == -1) {
        std::cout << "Invalid\n";
        return;
    }
    if (!isbn.empty() && booksystem_.CheckExist(isbn)) {
        std::cerr << "exist isbn\n";
        std::cout << "Invalid\n";
        return;
    }
    int pos = 0;
    std::string res = "modify ";
    for (auto op : ops) {
        if (op == ModifyOperator::ISBN) {
            booksystem_.ModifyIsbn(id, isbn);
            res += "-isbn=" + isbn.str() + " ";
        } else if (op == ModifyOperator::AUTHOR) {
            booksystem_.ModifyAuthor(id, others[pos++]);
            res += "-author=" + others[pos - 1].str() + " ";
        } else if (op == ModifyOperator::BOOKNAME) {
            booksystem_.ModifyName(id, others[pos++]);
            res += "-name=" + others[pos - 1].str() + " ";
        } else if (op == ModifyOperator::KEYWORD) {
            booksystem_.ModifyKeyword(id, others[pos++]);
            res += "-keyword=" + others[pos - 1].str() + " ";
        } else if (op == ModifyOperator::PRICE) {
            booksystem_.ModifyPrice(id, price);
            res += "-price=" + std::to_string(price) + " ";
        } 
    }
    logsystem_.AddLog(accountsystem_.Nowid(), logstring(res));
}
inline void Operator::Import(int num, double cost) {
    if (!CheckPriviledge(3)) {
        std::cout << "Invalid\n";
        return;
    }
    int id = accountsystem_.NowSelect();
    if (id == -1) {
        std::cout << "Invalid\n";
        return;
    }
    booksystem_.ImportBook(id, num);
    logsystem_.AddFinance(cost, -1);
    logsystem_.AddLog(accountsystem_.Nowid(), logstring("import " + std::to_string(num) + " " + std::to_string(cost)));
}

inline void Operator::ShowFinance(int count, bool is_all) {
    if (!CheckPriviledge(7)) {
        std::cout << "Invalid\n";
        return;
    }
    if (is_all) {
        logsystem_.PrintFinance(logsystem_.FinanceCount());
    } else {
        if (count > logsystem_.FinanceCount()) {
            std::cout << "Invalid\n";
            return;
        }
        logsystem_.PrintFinance(count);
    }
}

inline void Operator::Log() {
    logsystem_.Log();
}
inline void Operator::ReportFinance() {
    logsystem_.ReportFinance();
}
inline void Operator::ReportEmployee() {
    auto logs = logsystem_.AllLog();
    std::sort(logs.begin(), logs.end(), [&](const auto &x, const auto &y) {
        if (x.Userid() == y.Userid()) {
            return x.Time() < y.Time();
        }
        return x.Userid() < y.Userid();
    });
    std::cout << "EMPLOYEE WORKING SHEET\n";
    for (size_t i = 0; i < logs.size(); i++) {
        auto j = i;
        auto acc = accountsystem_.FindAccount(logs[i].Userid());
        if (acc.priviledge < 3) {
            continue;
        }
        std::cout << "EMPLOYEEID: " << acc.userid << " NAME: " << acc.username << "\n";
        while (j < logs.size() && logs[j].Userid() == logs[i].Userid()) {
            std::cout << logs[j].Context() << "\n";
            j++;
        }
        i = j - 1;
    }
}

#endif // OPERATOR_HPP