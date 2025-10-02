#include "Polynomial.hpp"
using namespace std;

// �ϲ�ͬ�����������
void Polynomial::simplify() {
    sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
        return a.exp > b.exp;
        });

    vector<Term> result;
    for (auto& t : terms) {
        if (!result.empty() && result.back().exp == t.exp) {
            result.back().coeff += t.coeff;
        }
        else {
            result.push_back(t);
        }
    }

    terms.clear();
    for (auto& t : result) {
        if (t.coeff != 0) terms.push_back(t);
    }
}

// �����������г�ʼ��
Polynomial::Polynomial(const vector<int>& data) {
    int n = data[0];
    for (int i = 0; i < n; i++) {
        int c = data[2 * i + 1];
        int e = data[2 * i + 2];
        terms.emplace_back(c, e);
    }
    simplify();
}

// �������ʽ
void Polynomial::input() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int c, e;
        cin >> c >> e;
        terms.emplace_back(c, e);
    }
    simplify();
}

// �������ʽ
void Polynomial::print() const {
    cout << terms.size();
    for (auto& t : terms) {
        cout << " " << t.coeff << " " << t.exp;
    }
    cout << endl;
}

// �ӷ�
Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial result;
    result.terms = terms;
    for (auto& t : other.terms) {
        result.terms.emplace_back(t.coeff, t.exp);
    }
    result.simplify();
    return result;
}

// ����
Polynomial Polynomial::sub(const Polynomial& other) const {
    Polynomial result;
    result.terms = terms;
    for (auto& t : other.terms) {
        result.terms.emplace_back(-t.coeff, t.exp);
    }
    result.simplify();
    return result;
}

// �˷�
Polynomial Polynomial::multiply(const Polynomial& other) const {
    Polynomial result;
    for (auto& a : terms) {
        for (auto& b : other.terms) {
            result.terms.emplace_back(a.coeff * b.coeff, a.exp + b.exp);
        }
    }
    result.simplify();
    return result;
}

// ����������ʽ��������
pair<Polynomial, Polynomial> Polynomial::divide(const Polynomial& other) const {
    Polynomial dividend = *this;
    Polynomial divisor = other;
    Polynomial quotient;

    if (divisor.terms.empty()) {
        throw runtime_error("���������ʽ��");
    }

    while (!dividend.terms.empty() && dividend.terms[0].exp >= divisor.terms[0].exp) {
        int c = dividend.terms[0].coeff / divisor.terms[0].coeff;
        int e = dividend.terms[0].exp - divisor.terms[0].exp;
        Polynomial termPoly;
        termPoly.terms.emplace_back(c, e);
        termPoly.simplify();

        quotient = quotient.add(termPoly);
        dividend = dividend.sub(termPoly.multiply(divisor));
        dividend.simplify();
    }
    return { quotient, dividend };
}

// �� x ����ֵ
long long Polynomial::evaluate(int x) const {
    long long result = 0;
    for (auto& t : terms) {
        long long val = 1;
        for (int i = 0; i < t.exp; i++) {
            val *= x;
        }
        result += t.coeff * val;
    }
    return result;
}
