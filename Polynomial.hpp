#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility> // for pair

class Polynomial {
private:
    struct Term {
        int coeff; // ϵ��
        int exp;   // ָ��
        Term(int c, int e) : coeff(c), exp(e) {}
    };
    std::vector<Term> terms;

    void simplify(); // �ϲ�ͬ�����ָ����������

public:
    Polynomial() = default;
    Polynomial(const std::vector<int>& data); // �����������г�ʼ��

    void input();       // �������ʽ
    void print() const; // �������ʽ

    Polynomial add(const Polynomial& other) const;     // �ӷ�
    Polynomial sub(const Polynomial& other) const;     // ����
    Polynomial multiply(const Polynomial& other) const;// �˷�
    std::pair<Polynomial, Polynomial> divide(const Polynomial& other) const; // ��������, ������

    long long evaluate(int x) const; // �� x ����ֵ
};

#endif
