#include "Polynomial.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

static bool nearlyEqual(double a, double b) {
    return fabs(a - b) <= Polynomial::EPS;
}

Polynomial::Polynomial(const vector<Term>& t) : terms(t) {
    normalize();
}

void Polynomial::setTerms(const vector<Term>& t) {
    terms = t;
    normalize();
}

void Polynomial::normalize() {
    // 删除系数接近0的项、合并指数接近的项、按指数降序
    // 1) 先过滤近零系数
    vector<Term> tmp;
    tmp.reserve(terms.size());
    for (auto& term : terms) {
        if (fabs(term.coeff) > EPS) tmp.push_back(term);
    }
    // 2) 按指数降序排序
    sort(tmp.begin(), tmp.end(), [](const Term& a, const Term& b) {
        return a.exp > b.exp;
        });
    // 3) 合并近似相同指数
    vector<Term> merged;
    for (auto& t : tmp) {
        if (!merged.empty() && nearlyEqual(merged.back().exp, t.exp)) {
            merged.back().coeff += t.coeff;
        }
        else {
            merged.push_back(t);
        }
    }
    // 4) 再次过滤因合并产生的近零系数
    terms.clear();
    for (auto& t : merged) {
        if (fabs(t.coeff) > EPS) terms.push_back(t);
    }
    // 保证零多项式 terms 为空（isZero 检测）
}

bool Polynomial::isZero() const {
    return terms.empty();
}

double Polynomial::degree() const {
    if (terms.empty()) return -numeric_limits<double>::infinity();
    return terms.front().exp;
}

double Polynomial::getCoeff(double exp) const {
    for (auto& t : terms) {
        if (nearlyEqual(t.exp, exp)) return t.coeff;
    }
    return 0.0;
}

Polynomial Polynomial::add(const Polynomial& other) const {
    vector<Term> acc = terms;
    for (auto& ot : other.terms) acc.emplace_back(ot.coeff, ot.exp);
    return Polynomial(acc);
}

Polynomial Polynomial::sub(const Polynomial& other) const {
    vector<Term> acc = terms;
    for (auto& ot : other.terms) acc.emplace_back(-ot.coeff, ot.exp);
    return Polynomial(acc);
}

Polynomial Polynomial::mul(const Polynomial& other) const {
    vector<Term> acc;
    acc.reserve(terms.size() * other.terms.size() + 1);
    for (auto& a : terms) {
        for (auto& b : other.terms) {
            double e = a.exp + b.exp;
            double c = a.coeff * b.coeff;
            acc.emplace_back(c, e);
        }
    }
    return Polynomial(acc);
}

pair<Polynomial, Polynomial> Polynomial::div(const Polynomial& divisor) const {
    if (divisor.isZero()) throw runtime_error("除以零多项式");

    // Copy remainder as vector<Term>
    vector<Term> R = terms;
    vector<Term> Q; // quotient terms (unsimplified)
    // make a helper to normalize R vector into Polynomial and get degree easily
    auto normalizeVec = [](vector<Term>& v) {
        // filter near-zero, sort desc, merge near exponents
        vector<Term> tmp;
        tmp.reserve(v.size());
        for (auto& t : v) if (fabs(t.coeff) > Polynomial::EPS) tmp.push_back(t);
        sort(tmp.begin(), tmp.end(), [](const Term& a, const Term& b) { return a.exp > b.exp; });
        vector<Term> merged;
        for (auto& t : tmp) {
            if (!merged.empty() && fabs(merged.back().exp - t.exp) <= Polynomial::EPS) merged.back().coeff += t.coeff;
            else merged.push_back(t);
        }
        v.swap(merged);
        };

    // prepare divisor normalized vector
    vector<Term> D = divisor.terms;
    normalizeVec(D);
    if (D.empty()) throw runtime_error("除以零多项式");

    // helper to get degree of vector (or -inf)
    auto vecDegree = [](const vector<Term>& v)->double {
        if (v.empty()) return -numeric_limits<double>::infinity();
        return v.front().exp;
        };

    normalizeVec(R);
    double db = vecDegree(D);
    double leadD = D.front().coeff;

    double dr = vecDegree(R);
    if (!(dr >= db - Polynomial::EPS)) {
        // degree less than divisor
        Polynomial q;
        Polynomial r(R);
        return { q, r };
    }

    // main loop
    while (!R.empty()) {
        normalizeVec(R);
        dr = vecDegree(R);
        if (!(dr >= db - Polynomial::EPS)) break;
        double leadR = R.front().coeff;
        double qExp = dr - db;           // quotient term exponent (may be fractional)
        double qCoeff = leadR / leadD;
        Q.emplace_back(qCoeff, qExp);

        // subtract qCoeff * x^qExp * D from R
        // build shifted version of D
        for (auto& dt : D) {
            double newExp = dt.exp + qExp;
            double newCoeff = dt.coeff * qCoeff;
            R.emplace_back(-newCoeff, newExp); // subtract
        }
        // normalize R for next iteration
        normalizeVec(R);
    }

    Polynomial quotient(Q);
    Polynomial remainder(R);
    return { quotient, remainder };
}

string Polynomial::formatNumber(double v) {
    double iv = round(v);
    if (fabs(v - iv) < EPS) {
        ostringstream oss;
        oss << (long long)iv;
        return oss.str();
    }
    else {
        ostringstream oss;
        oss << fixed << setprecision(6) << v;
        string s = oss.str();
        while (!s.empty() && s.back() == '0') s.pop_back();
        if (!s.empty() && s.back() == '.') s.pop_back();
        return s;
    }
}

string Polynomial::formatExp(double e) {
    double iv = round(e);
    if (fabs(e - iv) < EPS) {
        ostringstream oss;
        oss << (long long)iv;
        return oss.str();
    }
    else {
        ostringstream oss;
        oss << fixed << setprecision(6) << e;
        string s = oss.str();
        while (!s.empty() && s.back() == '0') s.pop_back();
        if (!s.empty() && s.back() == '.') s.pop_back();
        return s;
    }
}

string Polynomial::toString() const {
    if (isZero()) return "0";
    ostringstream oss;
    bool first = true;
    for (auto& t : terms) {
        double exp = t.exp;
        double coeff = t.coeff;
        if (fabs(coeff) < EPS) continue;
        string coeffStr = formatNumber(fabs(coeff));
        string expStr = formatExp(exp);
        // build term string without leading sign
        string termStr;
        if (fabs(exp) < EPS) {
            termStr = coeffStr;
        }
        else {
            bool coeffIsOne = (fabs(fabs(coeff) - 1.0) < EPS);
            if (!coeffIsOne) termStr += coeffStr;
            termStr += "x";
            if (!(fabs(exp - 1.0) < EPS)) termStr += "^" + expStr;
        }
        if (first) {
            if (coeff < 0) oss << "-";
            oss << termStr;
            first = false;
        }
        else {
            if (coeff < 0) oss << " - " << termStr;
            else oss << " + " << termStr;
        }
    }
    if (first) return "0";
    return oss.str();
}

void Polynomial::print(ostream& out) const {
    out << toString();
}

double Polynomial::evaluate(double x) const {
    if (isZero()) return 0.0;
    // If any term has negative exponent and x == 0 -> error
    for (auto& t : terms) {
        if (t.exp < 0 && fabs(x) < EPS) {
            throw runtime_error("多项式包含负指数项，x = 0 无法求值（会除以零）");
        }
        // if x < 0 and exponent not integer, pow may be nan -> check
        if (x < 0) {
            double rounded = round(t.exp);
            if (fabs(t.exp - rounded) > EPS) {
                throw runtime_error("在 x < 0 且指数为非整数时，结果为复数，本程序不支持复数运算");
            }
        }
    }
    double res = 0.0;
    for (auto& t : terms) {
        double termVal = t.coeff * pow(x, t.exp);
        res += termVal;
    }
    return res;
}
