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
    cout << "【按次数输入 — 推荐给新手】\n";
    cout << "请输入多项式的最高次数 n（非负整数，例如 n=3 表示最高项 x^3）:\n";
    int n;
    while (!(in >> n) || n < 0) {
        cout << "输入无效，请输入非负整数次数 n: ";
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "接下来请依次输入系数（从 x^" << n << " 到 x^0），可为整数（包括 0 和负数）。\n";
    for (int e = n; e >= 0; --e) {
        cout << "系数 a_" << e << " (x^" << e << "): ";
        long long c;
        while (!(in >> c)) {
            cout << "输入无效，请输入整数系数: ";
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (c != 0) terms.emplace_back(c, e);
    }
    simplify();
    cout << "输入完成。\n";
}

void Polynomial::inputBySequence(istream& in) {
    terms.clear();
    cout << "【序列输入 — 兼容原始格式】\n";
    cout << "请输入序列：n c1 e1 c2 e2 ... (n 为项数)\n";
    int n;
    while (!(in >> n) || n < 0) {
        cout << "输入无效，请输入非负整数 n: ";
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    for (int i = 0; i < n; ++i) {
        long long c; int e;
        while (!(in >> c >> e)) {
            cout << "读取项失败，请重新输入该项的 系数 指数: ";
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (c != 0) terms.emplace_back(c, e);
    }
    simplify();
    cout << "序列输入完成。\n";
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
            // 首项：直接输出正负
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
            // 后续项：带符号空格
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
    if (divisor.isZero()) throw runtime_error("除以零多项式错误。");
    Polynomial dividend = *this;
    Polynomial q; // 商
    // 长除法（整系数）：只有当首项系数能整除时才进行该项的消去；否则停止，剩余作为余数
    while (!dividend.isZero() && dividend.maxExp() >= divisor.maxExp()) {
        long long a = dividend.terms.front().coeff;
        long long b = divisor.terms.front().coeff;
        int expDiff = dividend.terms.front().exp - divisor.terms.front().exp;
        if (b == 0) throw runtime_error("除式首项系数为0（非法）");
        // 只在整除时继续
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
    // 剩下的 dividend 为余数
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
