#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <limits>

class Polynomial {
public:
    // 全局容差（浮点比较），供所有文件使用
    static constexpr double EPS = 1e-9;

    struct Term {
        double exp;   // exponent (can be fractional, can be negative)
        double coeff; // coefficient
        Term(double c = 0.0, double e = 0.0) : exp(e), coeff(c) {}
    };

private:
    // 按指数降序排列的项（normalize 后保持）
    std::vector<Term> terms;

    void normalize(); // 合并近似相同 exponent 的项、删除近似 0 的项，并按指数降序排列

    // NOTE: formatNumber / formatExp moved to public so external code (Menu.cpp) can call them.

public:
    Polynomial() = default;
    explicit Polynomial(const std::vector<Term>& t);

    // 设置项（会 normalize）
    void setTerms(const std::vector<Term>& t);

    bool isZero() const;

    // 返回最高指数（若零多项式则返回 -inf）
    double degree() const;

    // 获取某次系数（若没有近似相同指数则返回 0）
    double getCoeff(double exp) const;

    // 基本运算（指数可为浮点）
    Polynomial add(const Polynomial& other) const;
    Polynomial sub(const Polynomial& other) const;
    Polynomial mul(const Polynomial& other) const;
    // 长除法（商与余数），支持浮点指数
    std::pair<Polynomial, Polynomial> div(const Polynomial& divisor) const;

    // 求值（如果存在负指数且 x == 0，会抛出异常）
    double evaluate(double x) const;

    // 输出
    std::string toString() const;
    void print(std::ostream& out = std::cout) const;

    // 供外部格式化数值（整数/小数）使用（已设为 public）
    static std::string formatNumber(double v);
    static std::string formatExp(double e);
};

#endif
