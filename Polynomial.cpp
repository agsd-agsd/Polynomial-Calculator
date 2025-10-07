#include "Polynomial.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

static const double EPS = 1e-9;

void Polynomial::normalize() {
    while (!coeffs.empty() && fabs(coeffs.back()) < EPS) coeffs.pop_back();
    if (coeffs.empty()) coeffs.push_back(0.0);
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

string Polynomial::termToString(double coeff, int exp) {
    string cstr = formatNumber(fabs(coeff));
    ostringstream oss;
    if (exp == 0) {
        oss << cstr;
        return oss.str();
    }
    if (fabs(fabs(coeff) - 1.0) < EPS) {
        oss << "x";
    }
    else {
        oss << cstr << "x";
    }
    if (exp != 1) oss << "^" << exp;
    return oss.str();
}

Polynomial::Polynomial(const vector<double>& c) : coeffs(c) {
    normalize();
}

void Polynomial::setCoeffs(const vector<double>& c) {
    coeffs = c;
    normalize();
}

int Polynomial::degree() const {
    int last = (int)coeffs.size() - 1;
    while (last >= 0 && fabs(coeffs[last]) < EPS) --last;
    if (last < 0) return 0;
    return last;
}

double Polynomial::getCoeff(int exp) const {
    if (exp < 0 || exp >= (int)coeffs.size()) return 0.0;
    return coeffs[exp];
}

Polynomial Polynomial::add(const Polynomial& other) const {
    int n = max(degree(), other.degree());
    vector<double> res(n + 1, 0.0);
    for (int i = 0; i <= n; ++i) res[i] = getCoeff(i) + other.getCoeff(i);
    return Polynomial(res);
}

Polynomial Polynomial::sub(const Polynomial& other) const {
    int n = max(degree(), other.degree());
    vector<double> res(n + 1, 0.0);
    for (int i = 0; i <= n; ++i) res[i] = getCoeff(i) - other.getCoeff(i);
    return Polynomial(res);
}

Polynomial Polynomial::mul(const Polynomial& other) const {
    int da = degree(), db = other.degree();
    vector<double> res(da + db + 1, 0.0);
    for (int i = 0; i <= da; ++i)
        for (int j = 0; j <= db; ++j)
            res[i + j] += getCoeff(i) * other.getCoeff(j);
    return Polynomial(res);
}

pair<Polynomial, Polynomial> Polynomial::div(const Polynomial& divisor) const {
    vector<double> A = coeffs;
    Polynomial D = divisor;
    D.normalize();
    if (D.coeffs.empty() || (D.coeffs.size() == 1 && fabs(D.coeffs[0]) < EPS))
        throw runtime_error("除以零多项式");
    int da = (int)A.size() - 1;
    while (da >= 0 && fabs(A[da]) < EPS) --da;
    int db = D.degree();
    if (da < db) {
        return { Polynomial(vector<double>{0.0}), Polynomial(A) };
    }
    vector<double> Q(da - db + 1, 0.0);
    vector<double> R = A;
    for (int k = da - db; k >= 0; --k) {
        double coefR = (k + db < (int)R.size()) ? R[k + db] : 0.0;
        double leadD = D.getCoeff(db);
        if (fabs(leadD) < EPS) throw runtime_error("除多项式首项为0");
        double qk = coefR / leadD;
        Q[k] = qk;
        for (int j = 0; j <= db; ++j) {
            int idx = j + k;
            if (idx >= (int)R.size()) R.resize(idx + 1, 0.0);
            R[idx] -= qk * D.getCoeff(j);
        }
    }
    Polynomial quotient(Q);
    Polynomial remainder(R);
    quotient.normalize();
    remainder.normalize();
    return { quotient, remainder };
}

double Polynomial::evaluate(double x) const {
    int d = degree();
    double res = 0.0;
    for (int i = d; i >= 0; --i) res = res * x + getCoeff(i);
    return res;
}

string Polynomial::toString() const {
    int last = (int)coeffs.size() - 1;
    while (last >= 0 && fabs(coeffs[last]) < EPS) --last;
    if (last < 0) return "0";
    ostringstream oss;
    bool first = true;
    for (int i = last; i >= 0; --i) {
        double c = getCoeff(i);
        if (fabs(c) < EPS) continue;
        if (first) {
            if (c < 0) oss << "-";
            oss << termToString(c, i);
            first = false;
        }
        else {
            if (c < 0) oss << " - ";
            else oss << " + ";
            oss << termToString(c, i);
        }
    }
    if (first) return "0";
    return oss.str();
}

void Polynomial::print(ostream& out) const {
    out << toString();
}
