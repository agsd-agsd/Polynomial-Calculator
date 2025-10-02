#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility> // for pair

class Polynomial {
private:
    struct Term {
        int coeff; // 系数
        int exp;   // 指数
        Term(int c, int e) : coeff(c), exp(e) {}
    };
    std::vector<Term> terms;

    void simplify(); // 合并同类项并按指数降序排列

public:
    Polynomial() = default;
    Polynomial(const std::vector<int>& data); // 根据输入序列初始化

    void input();       // 输入多项式
    void print() const; // 输出多项式

    Polynomial add(const Polynomial& other) const;     // 加法
    Polynomial sub(const Polynomial& other) const;     // 减法
    Polynomial multiply(const Polynomial& other) const;// 乘法
    std::pair<Polynomial, Polynomial> divide(const Polynomial& other) const; // 除法（商, 余数）

    long long evaluate(int x) const; // 在 x 处求值
};

#endif
