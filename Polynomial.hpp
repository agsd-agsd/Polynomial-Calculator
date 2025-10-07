// Polynomial.hpp (已修改：formatNumber 设为 public static)
#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utility>

class Polynomial {
private:
    std::vector<double> coeffs; // coeffs[i] 对应 x^i

    void normalize(); // 去掉高位近似为0的系数
    static std::string termToString(double coeff, int exp); // 单项表示

public:
    Polynomial() = default;
    explicit Polynomial(const std::vector<double>& c);

    void setCoeffs(const std::vector<double>& c);

    int degree() const;
    double getCoeff(int exp) const;

    Polynomial add(const Polynomial& other) const;
    Polynomial sub(const Polynomial& other) const;
    Polynomial mul(const Polynomial& other) const;
    std::pair<Polynomial, Polynomial> div(const Polynomial& divisor) const;

    double evaluate(double x) const;

    std::string toString() const;
    void print(std::ostream& out = std::cout) const;

    // 将 formatNumber 暴露为 public static，供外部（如 Menu.cpp）调用用于输出格式化数字
    static std::string formatNumber(double v);
};

#endif
