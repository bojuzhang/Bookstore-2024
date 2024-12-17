#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, size_t info_len = 4>
class MemoryRiver {
private:
    /* your code here */
    const size_t sizeofint = sizeof(int);
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    size_t len_{};
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    ~MemoryRiver() {
        if (!file.is_open()) {
            file.open(file_name, std::ios::in | std::ios::out);
        }
        int t = len_;
        write_info(t, 1);
        file.close();
    }

    void initialise(string FN = "", bool clear_file = 0) {
        if (FN != "") file_name = FN;
        if (clear_file == 0) {
            file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        } else {
            file.open(file_name, std::ios::in);
            if (!file) {
                file.open(file_name, std::ios::out);
                int tmp = 0;
                for (int i = 0; i < info_len; ++i)
                    file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            } else {
                int t;
                get_info(t, 1);
                len_ = t;
            }
        }
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, size_t n) {
        if (n > info_len) return;
        if (!file.is_open()) {
            file.open(file_name, std::ios::in | std::ios::out);
        }
        file.seekg((n - 1) * sizeofint);
        file.read(reinterpret_cast<char *>(&tmp), sizeofint);
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, size_t n) {
        if (n > info_len) return;
        if (!file.is_open()) {
            file.open(file_name, std::ios::in | std::ios::out);
        }
        file.seekp((n - 1) * sizeofint);
        file.write(reinterpret_cast<char *>(&tmp), sizeofint);
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    size_t write(T &t) {
        if (!file.is_open()) {
            file.open(file_name, std::ios::in | std::ios::out);
        }
        size_t pos = info_len * sizeofint + len_ * sizeofT;
        file.seekp(pos);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        ++len_;
        return len_ - 1;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const size_t index) {
        if (!file.is_open()) {
            file.open(file_name, std::ios::in | std::ios::out);
        }
        size_t pos = info_len * sizeofint + index * sizeofT;
        file.seekp(pos);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const size_t index) {
        if (!file.is_open()) {
            file.open(file_name, std::ios::in | std::ios::out);
        }
        size_t pos = info_len * sizeofint + index * sizeofT;
        file.seekg(pos);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
    }

    // //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    // void Delete(int index) {
    //     /* your code here */
    // }
};


#endif //BPT_MEMORYRIVER_HPP