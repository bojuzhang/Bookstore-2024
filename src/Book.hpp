#pragma once
#ifndef BOOK_HPP
#define BOOK_HPP

#include <cstddef>
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

    // Book() = delete;
    Book() {};
    Book(int id_, const string20 &s) : id(id_), isbn(s) {};
    Book(const Book &other) {
        isbn = other.isbn;
        bookname = other.bookname;
        author = other.author;
        keyword = other.keyword;
        id = other.id;
        rem = other.rem;
        price = other.price;
    }
    Book& operator = (const Book &other) {
        isbn = other.isbn;
        bookname = other.bookname;
        author = other.author;
        keyword = other.keyword;
        id = other.id;
        rem = other.rem;
        price = other.price;
        return *this;
    }

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
    string60 tmp;
    for (size_t i = 0; i < keyword.size(); i++) {
        auto c = keyword[i];
        if (c == '|') {
            keywords.push_back(tmp);
            tmp.clear();
        } else {
            tmp.push_back(c);
        }
    }
    if (tmp.size() > 0) {
        keywords.push_back(tmp);
    }
    return keywords;
};

class BookSystem {
public:
    BookSystem() : idtobook_("id_to_book"), 
    isbntoid_("isbn_to_id"), authortoid_("author_to_id"), 
    booknametoid_("bookname_to_id"), keywordtoid_("keyword_to_id") {
        cnt_book_ = idtobook_.size();
        idx_ = idtobook_.queryadded();
    }

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
    void BuyBook(int, int);
    void ImportBook(int, int);

    void ModifyIsbn(int, const string20 &);
    void ModifyName(int, const string60 &);
    void ModifyAuthor(int, const string60 &);
    void ModifyKeyword(int, const string60 &);
    void ModifyPrice(int, double);

    int FindId(const string20 &);
    Book FindBook(int);
    bool CheckExist(const string20 &);
private:
    BlockList<int, Book> idtobook_;
    BlockList<string20, int> isbntoid_;
    BlockList<string60, int> authortoid_, booknametoid_, keywordtoid_;
    int cnt_book_;
    int idx_;
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
    stable_sort(res.begin(), res.end(), [&](auto a, auto b) {
        return a.isbn < b.isbn;
    });
    if (!res.size()) {
        std::cout << "\n";
    }
    for (const auto &p : res) {
        std::cerr << "testasshfsdhf " << p << "\n";
        std::cout << p;
    }
}
inline void BookSystem::ShowAuthor(const string60 &author) {
    auto res = QueryAuthor(author);
    stable_sort(res.begin(), res.end(), [&](auto a, auto b) {
        return a.isbn < b.isbn;
    });
    if (!res.size()) {
        std::cout << "\n";
    }
    for (const auto &p : res) {
        std::cout << p;
    }
}
inline void BookSystem::ShowName(const string60 &bookname) {
    auto res = QueryName(bookname);
    stable_sort(res.begin(), res.end(), [&](auto a, auto b) {
        return a.isbn < b.isbn;
    });
    if (!res.size()) {
        std::cout << "\n";
    }
    for (const auto &p : res) {
        std::cout << p;
    }
}
inline void BookSystem::ShowKeyword(const string60 &keyword) {
    auto res = QueryKeyword(keyword);
    stable_sort(res.begin(), res.end(), [&](auto a, auto b) {
        return a.isbn < b.isbn;
    });
    if (!res.size()) {
        std::cout << "\n";
    }
    for (const auto &p : res) {
        std::cout << p;
    }
}
inline void BookSystem::ShowAll() {
    auto res = QueryAll();
    stable_sort(res.begin(), res.end(), [&](auto a, auto b) {
        return a.isbn < b.isbn;
    });
    if (!res.size()) {
        std::cout << "\n";
    }
    for (const auto &p : res) {
        std::cout << p;
    }
}

inline void BookSystem::AddBook(const string20 &isbn) {
    int id = idtobook_.queryadded() + 1;
    idtobook_.modifyadded(id);
    std::cerr << "testid: !!! : " << id << "\n";
    cnt_book_++;
    Book p(id, isbn);
    idtobook_.Insert(std::make_pair(id, p));
    isbntoid_.Insert(std::make_pair(isbn, id));
    booknametoid_.Insert(std::make_pair(p.bookname, id));
    authortoid_.Insert(std::make_pair(p.author, id));
    keywordtoid_.Insert(std::make_pair(p.keyword, id));
}
inline void BookSystem::BuyBook(int id, int num) {
    auto p = FindBook(id);
    idtobook_.Delete(std::make_pair(id, p));
    p.rem -= num;
    idtobook_.Insert(std::make_pair(id, p));
}
inline void BookSystem::ImportBook(int id, int num) {
    auto p = FindBook(id);
    idtobook_.Delete(std::make_pair(id, p));
    p.rem += num;
    idtobook_.Insert(std::make_pair(id, p));
}

inline void BookSystem::ModifyIsbn(int id, const string20 &isbn) {
    auto p = FindBook(id);
    std::cerr << "tets " << p.isbn << " " << isbn << "\n";
    isbntoid_.Delete(std::make_pair(p.isbn, id));
    idtobook_.Delete(std::make_pair(id, p));
    p.isbn = isbn;
    isbntoid_.Insert(std::make_pair(p.isbn, id));
    idtobook_.Insert(std::make_pair(id, p));
    std::cerr << CheckExist(isbn) << "\n";
    auto tmp = QueryIsbn(isbn)[0];
    std::cerr << "testmodifysucc " << tmp << "\n";
}
inline void BookSystem::ModifyName(int id, const string60 &bookname) {
    auto p = FindBook(id);
    booknametoid_.Delete(std::make_pair(p.bookname, id));
    idtobook_.Delete(std::make_pair(id, p));
    p.bookname = bookname;
    booknametoid_.Insert(std::make_pair(p.bookname, id));
    idtobook_.Insert(std::make_pair(id, p));
    std::cerr << "testmodifysucc " << QueryName(bookname)[0];
}
inline void BookSystem::ModifyAuthor(int id, const string60 &author) {
    auto p = FindBook(id);
    authortoid_.Delete(std::make_pair(p.author, id));
    idtobook_.Delete(std::make_pair(id, p));
    p.author = author;
    authortoid_.Insert(std::make_pair(p.author, id));
    idtobook_.Insert(std::make_pair(id, p));
}
inline void BookSystem::ModifyKeyword(int id, const string60 &keyword) {
    auto p = FindBook(id);
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
inline void BookSystem::ModifyPrice(int id, double price) {
    auto p = FindBook(id);
    idtobook_.Delete(std::make_pair(id, p));
    p.price = price;
    idtobook_.Insert(std::make_pair(id, p));
}

inline Book BookSystem::FindBook(int id) {
    return idtobook_.Find(id)[0];
}
inline int BookSystem::FindId(const string20 &isbn) {
    return isbntoid_.Find(isbn)[0];
}
inline bool BookSystem::CheckExist(const string20 &isbn) {
    return !isbntoid_.Find(isbn).empty();
}

#endif