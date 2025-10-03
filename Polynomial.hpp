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
    std::vector<Term> terms; // ���ְ�ָ�������Һϲ�������ϵ��

    void simplify(); // �ϲ�ͬ�����ָ����������
    bool isZero() const;
    int maxExp() const;

public:
    Polynomial() = default;
    Polynomial(const std::vector<long long>& seq); // ���и�ʽ��ʼ��

    // ���뷽ʽ�������� / �����У�����ԭʼ��ʽ��
    void inputByDegree(std::istream& in = std::cin);   // �����Ƽ���������������ٴӸߵ�������ϵ��
    void inputBySequence(std::istream& in = std::cin); // ԭʼ��n c1 e1 c2 e2 ...

    // ��������ָ�ʽ
    void printSequence(std::ostream& out = std::cout) const; // n c1 e1 ...
    std::string toPrettyString() const;                      // ���� ax^n + ... �ַ���
    void printPretty(std::ostream& out = std::cout) const;   // ��� pretty �ַ���

    // ��������
    Polynomial add(const Polynomial& other) const;
    Polynomial sub(const Polynomial& other) const;
    Polynomial multiply(const Polynomial& other) const;
    // ��������ϵ���������������޷�����ĳ����leading coeff ������������ֹͣ���ѵ�ǰ dividend ����������
    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const;

    // ��ֵ
    long long evaluate(long long x) const;
};

#endif
