#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>

class Polynomial {
private:
    struct Term {
        long long coeff;
        int exp;
        Term(long long c = 0, int e = 0) : coeff(c), exp(e) {}
    };
    std::vector<Term> terms; // 保持按指数降序且合并后无零系数

    void simplify(); // 合并同类项并按指数降序排列
    bool isZero() const;
    int maxExp() const;

public:
    Polynomial() = default;
    Polynomial(const std::vector<long long>& seq); // 序列格式初始化

    // 输入方式：按次数 / 按序列（兼容原始格式）
    void inputByDegree(std::istream& in = std::cin);   // 新手推荐：先输入次数，再从高到低输入系数
    void inputBySequence(std::istream& in = std::cin); // 原始：n c1 e1 c2 e2 ...

    // 输出：两种格式
    void printSequence(std::ostream& out = std::cout) const; // n c1 e1 ...
    std::string toPrettyString() const;                      // 返回 ax^n + ... 字符串
    void printPretty(std::ostream& out = std::cout) const;   // 输出 pretty 字符串

    // 基本运算
    Polynomial add(const Polynomial& other) const;
    Polynomial sub(const Polynomial& other) const;
    Polynomial multiply(const Polynomial& other) const;
    // 除法（整系数长除法）。若无法整除某步（leading coeff 非整除），则停止并把当前 dividend 当余数返回
    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const;

    // 求值
    long long evaluate(long long x) const;
};

#endif
