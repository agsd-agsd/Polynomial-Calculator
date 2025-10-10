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
    static constexpr double EPS = 1e-9;

    struct Term {
        double coeff;
        double exp;
        Term(double c = 0.0, double e = 0.0) : coeff(c), exp(e) {}
    };

private:
    std::vector<Term> terms; // 保持按指数降序

    void normalize(); // 合并近似相同指数，移除近零系数并排序

public:
    Polynomial() = default;
    explicit Polynomial(const std::vector<Term>& t);

    void setTerms(const std::vector<Term>& t);
    bool isZero() const;
    double degree() const;
    double getCoeff(double exp) const;

    Polynomial add(const Polynomial& other) const;
    Polynomial sub(const Polynomial& other) const;
    Polynomial mul(const Polynomial& other) const;
    std::pair<Polynomial, Polynomial> div(const Polynomial& divisor) const;

    double evaluate(double x) const;

    std::string toString() const;
    void print(std::ostream& out = std::cout) const;

    std::string toSequence() const;

    static std::string formatNumber(double v);
    static std::string formatExp(double e);
};

#endif
