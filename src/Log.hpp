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

    virtual void Print();
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
        finance_log_.initialise("finance_log_file");
        finance_log_.get_info(history_count_, 1);
    }
    // tp: 1--> income -1-->outcome
    void AddFinance(double, int);
    int FinanceCount();
    void PrintFinance(int);
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
    << "+ " << now.first - p.first << " - " << now.second - p.second << "\n";
}

#endif