#include "Polynomial.hpp"
#include <sstream>
#include <stdexcept>
#include <cmath>

using namespace std;

static long long powLL(long long base, int exp) {
    long long res = 1;
    long long b = base;
    int e = exp;
    while (e > 0) {
        if (e & 1) res = res * b;
        b = b * b;
        e >>= 1;
    }
    return res;
}

void Polynomial::simplify() {
    if (terms.empty()) return;
    sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
        return a.exp > b.exp;
        });
    vector<Term> merged;
    merged.reserve(terms.size());
    for (auto& t : terms) {
        if (!merged.empty() && merged.back().exp == t.exp) {
            merged.back().coeff += t.coeff;
        }
        else {
            merged.push_back(t);
        }
    }
    terms.clear();
    for (auto& t : merged) {
        if (t.coeff != 0) terms.push_back(t);
    }
}

bool Polynomial::isZero() const {
    return terms.empty();
}

int Polynomial::maxExp() const {
    if (terms.empty()) return -1000000;
    return terms.front().exp;
}

Polynomial::Polynomial(const vector<long long>& seq) {
    if (seq.empty()) return;
    int n = static_cast<int>(seq[0]);
    for (int i = 0; i < n; ++i) {
        long long c = seq[1 + 2 * i];
        int e = static_cast<int>(seq[1 + 2 * i + 1]);
        terms.emplace_back(c, e);
    }
    simplify();
}

void Polynomial::inputByDegree(istream& in) {
    terms.clear();
    cout << "������������ �� �Ƽ������֡�\n";
    cout << "���������ʽ����ߴ��� n���Ǹ����������� n=3 ��ʾ����� x^3��:\n";
    int n;
    while (!(in >> n) || n < 0) {
        cout << "������Ч��������Ǹ��������� n: ";
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "����������������ϵ������ x^" << n << " �� x^0������Ϊ���������� 0 �͸�������\n";
    for (int e = n; e >= 0; --e) {
        cout << "ϵ�� a_" << e << " (x^" << e << "): ";
        long long c;
        while (!(in >> c)) {
            cout << "������Ч������������ϵ��: ";
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (c != 0) terms.emplace_back(c, e);
    }
    simplify();
    cout << "������ɡ�\n";
}

void Polynomial::inputBySequence(istream& in) {
    terms.clear();
    cout << "���������� �� ����ԭʼ��ʽ��\n";
    cout << "���������У�n c1 e1 c2 e2 ... (n Ϊ����)\n";
    int n;
    while (!(in >> n) || n < 0) {
        cout << "������Ч��������Ǹ����� n: ";
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    for (int i = 0; i < n; ++i) {
        long long c; int e;
        while (!(in >> c >> e)) {
            cout << "��ȡ��ʧ�ܣ��������������� ϵ�� ָ��: ";
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (c != 0) terms.emplace_back(c, e);
    }
    simplify();
    cout << "����������ɡ�\n";
}

void Polynomial::printSequence(ostream& out) const {
    out << terms.size();
    for (auto& t : terms) {
        out << " " << t.coeff << " " << t.exp;
    }
    out << "\n";
}

std::string Polynomial::toPrettyString() const {
    if (terms.empty()) return "0";
    ostringstream oss;
    bool first = true;
    for (auto& t : terms) {
        long long c = t.coeff;
        int e = t.exp;
        if (first) {
            // ���ֱ���������
            if (c < 0) oss << "-";
            long long ac = llabs(c);
            if (e == 0) {
                oss << ac;
            }
            else {
                if (ac != 1) oss << ac;
                oss << "x";
                if (e != 1) oss << "^" << e;
            }
            first = false;
        }
        else {
            // ����������ſո�
            if (c < 0) oss << " - ";
            else oss << " + ";
            long long ac = llabs(c);
            if (e == 0) {
                oss << ac;
            }
            else {
                if (ac != 1) oss << ac;
                oss << "x";
                if (e != 1) oss << "^" << e;
            }
        }
    }
    return oss.str();
}

void Polynomial::printPretty(ostream& out) const {
    out << toPrettyString() << "\n";
}

Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial res;
    res.terms = terms;
    for (auto& t : other.terms) res.terms.emplace_back(t.coeff, t.exp);
    res.simplify();
    return res;
}

Polynomial Polynomial::sub(const Polynomial& other) const {
    Polynomial res;
    res.terms = terms;
    for (auto& t : other.terms) res.terms.emplace_back(-t.coeff, t.exp);
    res.simplify();
    return res;
}

Polynomial Polynomial::multiply(const Polynomial& other) const {
    Polynomial res;
    for (auto& a : terms) {
        for (auto& b : other.terms) {
            res.terms.emplace_back(a.coeff * b.coeff, a.exp + b.exp);
        }
    }
    res.simplify();
    return res;
}

pair<Polynomial, Polynomial> Polynomial::divide(const Polynomial& divisor) const {
    if (divisor.isZero()) throw runtime_error("���������ʽ����");
    Polynomial dividend = *this;
    Polynomial q; // ��
    // ����������ϵ������ֻ�е�����ϵ��������ʱ�Ž��и������ȥ������ֹͣ��ʣ����Ϊ����
    while (!dividend.isZero() && dividend.maxExp() >= divisor.maxExp()) {
        long long a = dividend.terms.front().coeff;
        long long b = divisor.terms.front().coeff;
        int expDiff = dividend.terms.front().exp - divisor.terms.front().exp;
        if (b == 0) throw runtime_error("��ʽ����ϵ��Ϊ0���Ƿ���");
        // ֻ������ʱ����
        if (a % b != 0) break;
        long long qc = a / b;
        Polynomial t;
        t.terms.emplace_back(qc, expDiff);
        t.simplify();
        q = q.add(t);
        Polynomial subtrahend = t.multiply(divisor);
        dividend = dividend.sub(subtrahend);
        dividend.simplify();
    }
    // ʣ�µ� dividend Ϊ����
    return { q, dividend };
}

long long Polynomial::evaluate(long long x) const {
    long long res = 0;
    for (auto& t : terms) {
        long long termVal = powLL(x, t.exp);
        res += t.coeff * termVal;
    }
    return res;
}
