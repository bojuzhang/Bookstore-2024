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
    void Modify(const std::vector<ModifyOperator> &, const std::vector<string60> &, const string20 &, double);
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
    if (password.empty() && accountsystem_.NowPriviledge() <= user.priviledge) {
        std::cout << "Invalid\n";
        return;
    }
    if (!password.empty() && user.password != password) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.Login(userid);
}
inline void Operator::Logout() {
    if (!CheckPriviledge(1)) return;
    if (!accountsystem_.size()) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.Logout();
}
inline void Operator::Register(const string30 &userid, const string30 &password, const string30 &username) {
    if (!CheckPriviledge(0)) return;
    if (accountsystem_.CheckExist(userid)) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.AddAccount(userid, username, password, 1);
}
inline void Operator::ModifyPassword
(const string30 &userid, const string30 &currentpassword, const string30 &newpassword) {
    if (!CheckPriviledge(1)) return;
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

}
inline void Operator::AddUser
(const string30 &userid, const string30 &password, int priviledge, const string30 &username) {
    if (!CheckPriviledge(3)) return;
    if (priviledge >= accountsystem_.NowPriviledge()) {
        std::cout << "Invalid\n";
        return;
    }
    if (accountsystem_.CheckExist(userid)) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.AddAccount(userid, username, password, priviledge);
}
inline void Operator::DeleteUser(const string30 &userid) {
    if (!CheckPriviledge(7)) return;
    if (!accountsystem_.CheckExist(userid)) {
        std::cout << "Invalid\n";
        return;
    }
    auto user = accountsystem_.FindAccount(userid);
    if (user.online_cnt > 0) {
        std::cout << "Invalid\n";
        return;
    }
    accountsystem_.DeleteAccount(user);
}

inline void Operator::Show(ShowOperator op, const string60 &other, const string20 &isbn) {
    if (!CheckPriviledge(1)) return;
    if (op == ShowOperator::ISBN) {
        booksystem_.ShowIsbn(isbn);
    } else if (op == ShowOperator::AUTHOR) {
        booksystem_.ShowAuthor(other);
    } else if (op == ShowOperator::BOOKNAME) {
        booksystem_.ShowName(other);
    } else if (op == ShowOperator::KEYWORD) {
        booksystem_.ShowKeyword(other);
    } else if (op == ShowOperator::ALL) {
        booksystem_.ShowAll();
    } else {
        std::cout << "Invalid\n";
        return;
    }
}
inline void Operator::BuyBook(const string20 &isbn, int num) {
    if (!CheckPriviledge(1)) return;
    if (!booksystem_.CheckExist(isbn)) {
        std::cout << "Invalid\n";
        return;
    }
    int id = booksystem_.FindId(isbn);
    auto book = booksystem_.FindBook(id);
    if (book.rem < num) {
        std::cout << "Invalid\n";
        return;
    }
    booksystem_.BuyBook(id, num);
    logsystem_.AddFinance(book.price * num, 1);
}
inline void Operator::Select(const string20 &isbn) {
    if (!CheckPriviledge(3)) return;
    if (!booksystem_.CheckExist(isbn)) {
        booksystem_.AddBook(isbn);
    }
    int id = booksystem_.FindId(isbn);
    accountsystem_.Select(id);
}
inline void Operator::Modify
(const std::vector<ModifyOperator> &ops, const std::vector<string60> &others, 
 const string20 &isbn, double price) {
    if (!CheckPriviledge(3)) return;
    int id = accountsystem_.NowSelect();
    if (id == -1) {
        std::cout << "Invalid\n";
        return;
    }
    if (!isbn.empty() && isbn == booksystem_.FindBook(id).isbn) {
        std::cout << "Invalid\n";
        return;
    }
    int pos = 0;
    for (auto op : ops) {
        if (op == ModifyOperator::ISBN) {
            booksystem_.ModifyIsbn(id, isbn);
        } else if (op == ModifyOperator::AUTHOR) {
            booksystem_.ModifyAuthor(id, others[pos++]);
        } else if (op == ModifyOperator::BOOKNAME) {
            booksystem_.ModifyName(id, others[pos++]);
        } else if (op == ModifyOperator::KEYWORD) {
            booksystem_.ModifyKeyword(id, others[pos++]);
        } else if (op == ModifyOperator::PRICE) {
            booksystem_.ModifyPrice(id, price);
        } 
    }
}
inline void Operator::Import(int num, double cost) {
    if (!CheckPriviledge(3)) return;
    int id = accountsystem_.NowSelect();
    if (id == -1) {
        std::cout << "Invalid\n";
        return;
    }
    booksystem_.ImportBook(id, num);
    logsystem_.AddFinance(cost, -1);
}

inline void Operator::ShowFinance(int count, bool is_all) {
    if (!CheckPriviledge(7)) return;
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

#endif // OPERATOR_HPP