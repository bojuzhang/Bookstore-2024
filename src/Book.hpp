#pragma once
#ifndef BOOK_HPP
#define BOOK_HPP

#include <iostream>
#include <utility>
#include <iomanip>
#include <ios>
#include "BlockList.hpp"
#include "MyVector.hpp"
#include <vector>

struct Book{
    string20 isbn{};
    string60 bookname{}, author{};
    string60 keyword{};
    int id{};
    int rem{};
    double price{};

    Book() = delete;
    Book(int id_, const string20 &s) : id(id_), isbn(s) {};

    bool operator < (const Book &) const;
    bool operator > (const Book &) const;
    bool operator == (const Book &) const;
    bool operator <= (const Book &) const;
    bool operator >= (const Book &) const;
    bool operator != (const Book &) const;
    
    friend std::ostream& operator << (std::ostream &os, const Book &v) {
        os << v.isbn << "\t" << v.bookname << "\t" << v.author << "\t" << v.keyword << "\t";
        os << std::fixed << std::setprecision(2) << v.price << "\t";
        os << v.rem << "\n";
        return os;
    }
};

// make the keyword to a vector of each keyword
inline std::vector<string60> ReleaseKeywords(const string60 &keyword) {
    std::vector<string60> keywords;
    // TODO
    return keywords;
};

class BookSystem {
public:
    BookSystem() : idtobook_("id_to_book"), 
    isbntoid_("isbn_to_id"), authortoid_("author_to_id"), 
    booknametoid_("bookname_to_id"), keywordtoid_("keyword_to_id") {}

    std::vector<Book> QueryIsbn(const string20 &);
    std::vector<Book> QueryName(const string60 &);
    std::vector<Book> QueryAuthor(const string60 &);
    std::vector<Book> QueryKeyword(const string60 &);
    std::vector<Book> QueryAll();
    void ShowIsbn(const string20 &);
    void ShowName(const string60 &);
    void ShowAuthor(const string60 &);
    void ShowKeyword(const string60 &);
    void ShowAll();

    void AddBook(const string20 &);

    void ModifyIsbn(int, const string20 &);
    void ModifyName(int, const string60 &);
    void ModifyAuthor(int, const string60 &);
    void ModifyKeyword(int, const string60 &);

    int FindId(const string20 &);
    Book FindBook(int);
private:
    BlockList<int, Book> idtobook_;
    BlockList<string20, int> isbntoid_;
    BlockList<string60, int> authortoid_, booknametoid_, keywordtoid_;
    int cnt_book_;
};

inline bool Book::operator < (const Book &other) const {
    if (id != other.id) {
        return id < other.id;
    }
    if (isbn != other.isbn) {
        return isbn < other.isbn;
    }
    if (bookname != other.bookname) {
        return bookname < other.bookname;
    }
    if (author != other.author) {
        return author < other.author;
    }
    if (keyword != other.keyword) {
        return keyword < other.keyword;
    }
    if (rem != other.rem) {
        return rem < other.rem;
    }
    if (price != other.price) {
        return price < other.price;
    }
    return false;
}
inline bool Book::operator > (const Book &other) const {
    if (id != other.id) {
        return id > other.id;
    }
    if (isbn != other.isbn) {
        return isbn > other.isbn;
    }
    if (bookname != other.bookname) {
        return bookname > other.bookname;
    }
    if (author != other.author) {
        return author > other.author;
    }
    if (keyword != other.keyword) {
        return keyword > other.keyword;
    }
    if (rem != other.rem) {
        return rem > other.rem;
    }
    if (price != other.price) {
        return price > other.price;
    }
    return false;
}
inline bool Book::operator == (const Book &other) const {
    if (id != other.id) {
        return false;
    }
    if (isbn != other.isbn) {
        return false;
    }
    if (bookname != other.bookname) {
        return false;
    }
    if (author != other.author) {
        return false;
    }
    if (keyword != other.keyword) {
        return false;
    }
    if (rem != other.rem) {
        return false;
    }
    if (price != other.price) {
        return false;
    }
    return true;
}
inline bool Book::operator <= (const Book &other) const {
    return !((*this) > other);
}
inline bool Book::operator >= (const Book &other) const {
    return !((*this) < other);
}
inline bool Book::operator != (const Book &other) const {
    return !((*this) != other);
}

inline std::vector<Book> BookSystem::QueryIsbn(const string20 &isbn) {
    auto ids = isbntoid_.Find(isbn);
    std::vector<Book> ans;
    ans.reserve(ids.size());
    for (const auto &p : ids) {
        ans.push_back(FindBook(p));
    }
    return ans;
}
inline std::vector<Book> BookSystem::QueryName(const string60 &name) {
    auto ids = booknametoid_.Find(name);
    std::vector<Book> ans;
    ans.reserve(ids.size());
    for (const auto &p : ids) {
        ans.push_back(FindBook(p));
    }
    return ans;
}
inline std::vector<Book> BookSystem::QueryAuthor(const string60 &author) {
    auto ids = authortoid_.Find(author);
    std::vector<Book> ans;
    ans.reserve(ids.size());
    for (const auto &p : ids) {
        ans.push_back(FindBook(p));
    }
    return ans;
}
inline std::vector<Book> BookSystem::QueryKeyword(const string60 &keyword) {
    // only one keyword
    auto ids = keywordtoid_.Find(keyword);
    std::vector<Book> ans;
    ans.reserve(ids.size());
    for (const auto &p : ids) {
        ans.push_back(FindBook(p));
    }
    return ans;
}
inline std::vector<Book> BookSystem::QueryAll() {
    return idtobook_.FindAll();
}
inline void BookSystem::ShowIsbn(const string20 &isbn) {
    auto res = QueryIsbn(isbn);
    for (const auto &p : res) {
        std::cout << p;
    }
}
inline void BookSystem::ShowAuthor(const string60 &author) {
    auto res = QueryAuthor(author);
    for (const auto &p : res) {
        std::cout << p;
    }
}
inline void BookSystem::ShowName(const string60 &bookname) {
    auto res = QueryName(bookname);
    for (const auto &p : res) {
        std::cout << p;
    }
}
inline void BookSystem::ShowKeyword(const string60 &keyword) {
    auto res = QueryKeyword(keyword);
    for (const auto &p : res) {
        std::cout << p;
    }
}
inline void BookSystem::ShowAll() {
    auto res = QueryAll();
    for (const auto &p : res) {
        std::cout << p;
    }
}

inline void BookSystem::AddBook(const string20 &isbn) {
    int id = ++cnt_book_;
    Book p(id, isbn);
    idtobook_.Insert(std::make_pair(id, p));
    isbntoid_.Insert(std::make_pair(isbn, id));
    booknametoid_.Insert(std::make_pair(p.bookname, id));
    authortoid_.Insert(std::make_pair(p.author, id));
    keywordtoid_.Insert(std::make_pair(p.keyword, id));
}

inline void BookSystem::ModifyIsbn(int id, const string20 &isbn) {
    auto p = idtobook_.Find(id)[0];
    isbntoid_.Delete(std::make_pair(p.isbn, id));
    idtobook_.Delete(std::make_pair(id, p));
    p.isbn = isbn;
    isbntoid_.Insert(std::make_pair(p.isbn, id));
    idtobook_.Insert(std::make_pair(id, p));
}
inline void BookSystem::ModifyName(int id, const string60 &bookname) {
    auto p = idtobook_.Find(id)[0];
    booknametoid_.Delete(std::make_pair(p.bookname, id));
    idtobook_.Delete(std::make_pair(id, p));
    p.bookname = bookname;
    booknametoid_.Insert(std::make_pair(p.bookname, id));
    idtobook_.Insert(std::make_pair(id, p));
}
inline void BookSystem::ModifyAuthor(int id, const string60 &author) {
    auto p = idtobook_.Find(id)[0];
    authortoid_.Delete(std::make_pair(p.author, id));
    idtobook_.Delete(std::make_pair(id, p));
    p.author = author;
    authortoid_.Insert(std::make_pair(p.author, id));
    idtobook_.Insert(std::make_pair(id, p));
}
inline void BookSystem::ModifyKeyword(int id, const string60 &keyword) {
    auto p = idtobook_.Find(id)[0];
    auto keywords = ReleaseKeywords(p.keyword);
    for (const auto &x : keywords) {
        keywordtoid_.Delete(std::make_pair(x, id));
    }
    idtobook_.Delete(std::make_pair(id, p));
    p.keyword = keyword;
    keywords = ReleaseKeywords(p.keyword);
    for (const auto &x : keywords) {
        keywordtoid_.Insert(std::make_pair(x, id));
    }
    idtobook_.Insert(std::make_pair(id, p));
}

inline Book BookSystem::FindBook(int id) {
    return idtobook_.Find(id)[0];
}
inline int BookSystem::FindId(const string20 &isbn) {
    return isbntoid_.Find(isbn)[0];
}

#endif