#pragma once
#include <cstddef>
#include <vector>
#ifndef LOG_HPP
#define LOG_HPP
#include "MemoryRiver.hpp"
#include "MyVector.hpp"
#include <iomanip>
#include <ios>
#include <iostream>
#include <utility>

using logstring = MyString<350>;

class LogBase {
private:
    string30 userid_{};
    logstring context_{};
    int time_{};
public:
    LogBase() {}
    LogBase(string30 userid, logstring context, int time) :
    userid_(userid), context_(context), time_(time) {}
    LogBase(const LogBase &other) {
        userid_ = other.userid_;
        context_ = other.context_;
        time_ = other.time_;
    }
    LogBase& operator = (const LogBase &other) {
        userid_ = other.userid_;
        context_ = other.context_;
        time_ = other.time_;
        return *this;
    }

    bool operator < (const LogBase &) const;
    bool operator > (const LogBase &) const;
    bool operator == (const LogBase &) const;
    bool operator <= (const LogBase &) const;
    bool operator >= (const LogBase &) const;
    bool operator != (const LogBase &) const;

    void Print() const {
        std::cout << "USER: " << userid_ << "\n";
        std::cout << "OPERATOR: " << context_ << "\n";
    }
    string30 Userid() const { return userid_; }
    logstring Context() const { return context_; }
    int Time() const { return time_; }
};

inline bool LogBase::operator < (const LogBase &other) const {
    return time_ < other.time_;
}
inline bool LogBase::operator > (const LogBase &other) const {
    return time_ > other.time_;
}
inline bool LogBase::operator == (const LogBase &other) const {
    return time_ == other.time_;
}
inline bool LogBase::operator <= (const LogBase &other) const {
    return time_ <= other.time_;
}
inline bool LogBase::operator >= (const LogBase &other) const {
    return time_ >= other.time_;
}
inline bool LogBase::operator != (const LogBase &other) const {
    return time_ != other.time_;
}


class LogSystem {
private:
    int history_count_;
    int time_;
    MemoryRiver<std::pair<double, double>> finance_log_;
    MemoryRiver<LogBase> whole_log_;
    // TODO: WHOLE LOG
public:
    LogSystem() {
        finance_log_.initialise("finance_log_file", 1);
        whole_log_.initialise("whole_log_", 1);
        finance_log_.get_info(history_count_, 1);
        whole_log_.get_info(time_, 1);
    }
    // tp: 1--> income -1-->outcome
    void AddFinance(double, int);
    int FinanceCount();
    void PrintFinance(int);
    void ReportFinance();
    std::vector<LogBase> AllLog();
    void Log();
    void AddLog(const string30 &, const logstring &);
};

inline void LogSystem::AddFinance(double x, int tp) {
    std::pair<double, double> p = {0.0, 0.0};
    if (history_count_) {
        finance_log_.read(p, history_count_ - 1);
    }
    if (tp == 1) {
        p.first += x;
    } else {
        p.second -= x;
    }
    history_count_++;
    finance_log_.write(p);
}
inline int LogSystem::FinanceCount() {
    return history_count_;
}
inline void LogSystem::PrintFinance(int count) {
    if (!count) {
        std::cout << "\n";
        return;
    }
    std::pair<double, double> now, p;
    finance_log_.read(now, history_count_ - 1);
    if (count < history_count_) {
        finance_log_.read(p, history_count_ - count - 1);
    } else {
        p = {0.0, 0.0};
    }
    std::cout << std::fixed << std::setprecision(2)
    << "+ " << now.first - p.first << " - " << std::abs(now.second - p.second) << "\n";
}

inline void LogSystem::ReportFinance() {
    std::cout << "HISTORY FINANCE LOG\n";
    std::pair<double, double> now, lst;
    now = lst = {0, 0};
    for (int i = 0; i < history_count_; i++) {
        finance_log_.read(now, i);
        std::cout << i + 1 << "-th OPERATION:\n";
        if (now.first > lst.first) {
            std::cout << "SALE:   " << std::setw(12) << std::fixed << std::setprecision(2) << now.first - lst.first << " "; 
        } else {
            std::cout << "IMPORT: " << std::setw(12) << std::fixed << std::setprecision(2) << lst.second - now.second << " ";
        }
        std::cout << "TOTALNOW: "  << std::fixed << std::setprecision(2)
    << "+ " << std::setw(12) << now.first << "   - " << std::setw(12) << std::abs(now.second) << "\n";
        lst = now;
    }
    std::cout << "\n";
    std::cout << "TOTALALL:" << std::setw(24)  << std::fixed << std::setprecision(2)
    << "+ " << std::setw(12) << lst.first << "   - " << std::setw(12) << std::abs(lst.second) << "\n";
}
inline std::vector<LogBase> LogSystem::AllLog() {
    int len;
    whole_log_.get_info(len, 1);
    std::vector<LogBase> ans;
    ans.reserve(len);
    LogBase p;
    for (int i = 0; i < len; i++) {
        whole_log_.read(p, i);
        ans.push_back(p);
    }
    return ans;
}
inline void LogSystem::Log() {
    auto p = AllLog();
    for (size_t i = 0; i < p.size(); i++) {
        std::cout << (i + 1) << "-th operator:\n";
        p[i].Print();
    }
}

inline void LogSystem::AddLog(const string30 &userid, const logstring &context) {
    auto p = LogBase(userid, context, ++time_);
    whole_log_.write(p);
}

#endif