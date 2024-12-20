#pragma once
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include "MyVector.hpp"
#include "BlockList.hpp"
#include <vector>
#include <utility>

struct Account {
    string30 userid, username, password;
    int priviledge;
    int online_cnt;

    bool operator < (const Account &) const;
    bool operator > (const Account &) const;
    bool operator == (const Account &) const;
    bool operator <= (const Account &) const;
    bool operator >= (const Account &) const;
    bool operator != (const Account &) const;

    Account() = delete;
    Account(const string30 &userid_, const string30 &username_, const string30 &password_, int priviledge_) :
    userid(userid_), username(username_), password(password_), priviledge(priviledge_) {}
};

class AccountSystem {
public:
    AccountSystem() : accountfile_("userid_to_account") {};
    bool CheckPassword(const string30 &, const string30 &);
    void Login(const string30 &);
    void Logout();
    void ModifyPassword(const string30 &, const string30 &);
    void AddAccount(const string30 &, const string30 &, const string30 &, int);
    void DeleteAccount(const Account &);
    void Select(int);
    int NowSelect();

    bool empty() { return stackaccount_.empty(); }
    size_t size() { return stackaccount_.size(); }

    Account FindAccount(const string30 &userid) {
        return accountfile_.Find(userid)[0];
    }
private:
    BlockList<string30, Account> accountfile_;
    std::vector<std::pair<Account, int>> stackaccount_;
};

inline bool Account::operator < (const Account &other) const {
    if (userid != other.userid) {
        return userid < other.userid;
    }
    if (username != other.username) {
        return username < other.username;
    }
    if (password != other.password) {
        return password < other.password;
    }
    if (priviledge != other.priviledge) {
        return priviledge < other.priviledge;
    }
    if (online_cnt != other.online_cnt) {
        return online_cnt < other.online_cnt;
    }
    return false;
}
inline bool Account::operator > (const Account &other) const {
    if (userid != other.userid) {
        return userid > other.userid;
    }
    if (username != other.username) {
        return username > other.username;
    }
    if (password != other.password) {
        return password > other.password;
    }
    if (priviledge != other.priviledge) {
        return priviledge > other.priviledge;
    }
    if (online_cnt != other.online_cnt) {
        return online_cnt > other.online_cnt;
    }
    return false;
}
inline bool Account::operator == (const Account &other) const {
    if (userid != other.userid) {
        return false;
    }
    if (username != other.username) {
        return false;
    }
    if (password != other.password) {
        return false;
    }
    if (priviledge != other.priviledge) {
        return false;
    }
    if (online_cnt != other.online_cnt) {
        return false;
    }
    return true;
}
inline bool Account::operator <= (const Account &other) const {
    return !((*this) > other);
}
inline bool Account::operator >= (const Account &other) const {
    return !((*this) < other);
}
inline bool Account::operator != (const Account &other) const {
    return !((*this) == other);
}

inline bool AccountSystem::CheckPassword(const string30 &userid, const string30 &password) {
    auto p = FindAccount(userid);
    return p.password == password;
}
inline void AccountSystem::Login(const string30 &userid) {
    auto p = FindAccount(userid);
    stackaccount_.push_back(std::make_pair(p, -1));
}
inline void AccountSystem::Logout() {
    stackaccount_.pop_back();
}
inline void AccountSystem::ModifyPassword(const string30 &userid, const string30 &new_password) {
    auto p = FindAccount(userid);
    accountfile_.Delete(std::make_pair(userid, p));
    p.password = new_password;
    accountfile_.Insert(std::make_pair(userid, p));
}
inline void AccountSystem::AddAccount
(const string30 &userid, const string30 &username, const string30 &password, int priviledge) {
    auto p = Account(userid, username, password, priviledge);
    accountfile_.Insert(std::make_pair(userid, p));
}
inline void AccountSystem::DeleteAccount(const Account &p) {
    accountfile_.Delete(std::make_pair(p.userid, p));
}

inline void AccountSystem::Select(int id) {
    stackaccount_.back().second = id;
}
inline int AccountSystem::NowSelect() {
    return stackaccount_.back().second;
}

#endif