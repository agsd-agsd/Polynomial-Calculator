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

    // 辅助：将指数数字转为 Unicode 上标字符串（例如 12 -> "¹²"）
    static std::string expToSuperscript(int e);

public:
    Polynomial() = default;
    Polynomial(const std::vector<long long>& seq); // 序列格式初始化

    // 输入方式：按次数输入（可以提供 presetDegree 以便父菜单先提示并传入）
    void inputByDegree(std::istream& in = std::cin, int presetDegree = -1);

    // 输出：机器序列 / 人类可读（漂亮）形式
    void printSequence(std::ostream& out = std::cout) const; // n c1 e1 ...
    std::string toPrettyString() const;                      // 返回 3x² - x + 2 之类字符串
    void printPretty(std::ostream& out = std::cout) const;   // 输出 pretty 字符串

    // 基本运算
    Polynomial add(const Polynomial& other) const;
    Polynomial sub(const Polynomial& other) const;
    Polynomial multiply(const Polynomial& other) const;
    // 除法（整系数长除法）。若无法整除某步（leading coeff 非整除），则停止并把当前 dividend 当余数返回
    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const;

    // 求值
    long long evaluate(long long x) const;

    // 供外部查看（调试/提示）
    bool empty() const { return terms.empty(); }
};

#endif
