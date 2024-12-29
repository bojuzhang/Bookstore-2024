#pragma once
#ifndef LOG_HPP
#define LOG_HPP
#include "MemoryRiver.hpp"
#include "MyVector.hpp"
#include <iomanip>
#include <ios>
#include <iostream>
#include <utility>

class LogBase {
private:
    string30 userid_;
    int time_;
public:
    LogBase() : time_(0), userid_("") {}
    LogBase(int time, string30 userid) :
    time_(time), userid_(userid) {}
    LogBase(const LogBase &other) {
        userid_ = other.userid_;
        time_ = other.time_;
    }
    virtual LogBase& operator = (const LogBase &other) {
        userid_ = other.userid_;
        time_ = other.time_;
        return *this;
    }

    virtual bool operator < (const LogBase &) const;
    virtual bool operator > (const LogBase &) const;
    virtual bool operator == (const LogBase &) const;
    virtual bool operator <= (const LogBase &) const;
    virtual bool operator >= (const LogBase &) const;
    virtual bool operator != (const LogBase &) const;

    virtual void Print() {
        std::cout << userid_ << " " << time_ << "\n";
    }
    virtual int Time() { return time_; }
    virtual string30 Userid() { return userid_; }
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

//TODO: LOGXXX

class LogSystem {
private:
    int history_count_;
    MemoryRiver<std::pair<double, double>> finance_log_;
    // TODO: WHOLE LOG
public:
    LogSystem() {
        finance_log_.initialise("finance_log_file", 1);
        finance_log_.get_info(history_count_, 1);
    }
    // tp: 1--> income -1-->outcome
    void AddFinance(double, int);
    int FinanceCount();
    void PrintFinance(int);
    void ReportFinance();
    void ReportEmployee();
    void Log();
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
inline void LogSystem::ReportEmployee() {

}
inline void LogSystem::Log() {

}

#endif