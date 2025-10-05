#include "Polynomial.hpp"
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <limits>

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

void Polynomial::inputByDegree(istream& in, int presetDegree) {
    terms.clear();
    int n = presetDegree;
    if (n < 0) {
        // 如果没有预设，则自己询问（兼容旧逻辑）
        cout << "请输入多项式的最高次数 n（非负整数，例如 n=3 表示最高项 x^3）。输入 -1 可取消： ";
        while (!(in >> n) || n < -1) {
            cout << "输入无效，请输入非负整数 n（或 -1 取消）： ";
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (n == -1) {
            cout << "已取消输入。\n";
            return;
        }
    }

    cout << "请依次输入系数（从 x^" << n << " 到 x^0），每步输入后回车。\n";
    vector<long long> tmpCoeffs(n + 1, 0);
    for (int e = n; e >= 0; --e) {
        cout << "系数 a_" << e << " (x^" << e << ")，输入 q 撤回并取消本次输入: ";
        // 我们只读取整数输入；提示用户先确认开始，若要撤回输入则输入 'q'（需要处理混合输入）
        // 为简单并稳妥：先读取 string，再尝试转换为整数或判断是否为 'q'
        string token;
        while (true) {
            if (!(in >> token)) {
                // 读入失败（例如 EOF），取消
                cout << "读取失败，取消输入。\n";
                terms.clear();
                return;
            }
            if (token == "q" || token == "Q") {
                cout << "撤回：已取消本次多项式输入。\n";
                terms.clear();
                return;
            }
            // 尝试将 token 转为 long long
            try {
                size_t idx = 0;
                long long val = stoll(token, &idx);
                if (idx != token.size()) {
                    throw invalid_argument("非纯整数");
                }
                tmpCoeffs[e] = val;
                break;
            }
            catch (...) {
                cout << "输入无效，请输入整数系数，或输入 q 取消: ";
            }
        }
    }

    // 将临时系数写入 terms（只有非零项）
    for (int e = n; e >= 0; --e) {
        long long c = tmpCoeffs[e];
        if (c != 0) terms.emplace_back(c, e);
    }
    simplify();

    // 显示并要求确认（撤回选项）
    cout << "你刚刚输入的多项式是： " << toPrettyString() << "\n";
    cout << "确认保存？(1=保存, 0=撤回并取消): ";
    int conf;
    while (!(in >> conf) || (conf != 0 && conf != 1)) {
        cout << "请输入 1 保存，或 0 撤回并取消: ";
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (conf == 0) {
        terms.clear();
        cout << "已撤回并取消保存。\n";
    }
    else {
        cout << "已保存多项式。\n";
    }
}

void Polynomial::printSequence(ostream& out) const {
    out << terms.size();
    for (auto& t : terms) {
        out << " " << t.coeff << " " << t.exp;
    }
    out << "\n";
}

// ASCII 兼容的指数显示（不会使用 Unicode 上标）
// 把上标输出为 ^n 形式（例如 x^2），x^1 则输出 x
static std::string expToAscii(int e) {
    if (e <= 0) return "";            // e == 0: 没有 x 部分
    if (e == 1) return "x";           // x^1 -> x
    return std::string("x^") + std::to_string(e);
}

std::string Polynomial::toPrettyString() const {
    if (terms.empty()) return "0";
    std::ostringstream oss;
    bool first = true;
    for (auto& t : terms) {
        long long c = t.coeff;
        int e = t.exp;
        if (first) {
            // 首项直接写符号
            if (c < 0) oss << "-";
            long long ac = std::llabs(c);
            if (e == 0) {
                oss << ac;
            }
            else {
                if (ac != 1) oss << ac;
                oss << expToAscii(e);
            }
            first = false;
        }
        else {
            if (c < 0) oss << " - ";
            else oss << " + ";
            long long ac = std::llabs(c);
            if (e == 0) {
                oss << ac;
            }
            else {
                if (ac != 1) oss << ac;
                oss << expToAscii(e);
            }
        }
    }
    return oss.str();
}

void Polynomial::printPretty(std::ostream& out) const {
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
