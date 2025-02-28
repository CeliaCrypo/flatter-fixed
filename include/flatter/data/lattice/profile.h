#pragma once

#include <memory>
#include <iostream>

namespace flatter {

class Profile {
public:
    Profile();
    Profile(unsigned int n);

    bool is_valid() const;

    double &operator[](unsigned int i);
    const double &operator[](unsigned int i) const;

    Profile subprofile(unsigned int start, unsigned int end);
    
    double get_drop() const;
    double get_spread() const;//如果想要输出spread和drop，请在具体算法实现处进行实现，防止反复运算，浪费资源

    void print_profile() const; //添加的输出profile这个向量的函数，向量不同分量之间用空格隔开。

private:
    bool is_valid_;
    std::shared_ptr<double[]> profile_elems;
    unsigned int n;
};

}