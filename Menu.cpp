#include "Menu.hpp"
#include "Polynomial.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <vector>

using namespace std;

static void clearStdin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static bool readDegree(int& outDegree) {
    while (true) {
        string tok;
        if (!(cin >> tok)) return false;
        try {
            int v = stoi(tok);
            outDegree = v;
            return true;
        }
        catch (...) {
            cout << "输入无效，请输入整数（或 -1 取消）：";
        }
    }
}

static int readCoeffOrCancel(double& value) {
    string tok;
    if (!(cin >> tok)) return -1;
    if (tok == "q" || tok == "Q") return 1;
    try {
        value = stod(tok);
        return 0;
    }
    catch (...) {
        return -1;
    }
}

static vector<double> inputPolynomialCoeffsInteractive(int degree) {
    vector<double> tmp(degree + 1, 0.0);
    cout << "请依次输入各项系数（从 x^" << degree << " 到 x^0），输入 q 撤回并取消本次多项式输入。\n";
    for (int e = degree; e >= 0; --e) {
        while (true) {
            cout << "系数 x^" << e << " : ";
            double val;
            int rc = readCoeffOrCancel(val);
            if (rc == -1) {
                cout << "输入无效，请重新输入或输入 q 取消。\n";
                clearStdin();
                continue;
            }
            else if (rc == 1) {
                cout << "已选择取消当前多项式输入。\n";
                return vector<double>();
            }
            else {
                tmp[e] = val;
                break;
            }
        }
    }
    return tmp;
}

static int offerCancelOptionsForAorB(const string& name) {
    while (true) {
        cout << "你选择了撤回/取消。\n";
        cout << "请选择：1. 重新输入 " << name << "  2. 输入多项式 " << (name == "a" ? "b" : "a") << "  3. 返回主菜单\n";
        cout << "请输入选项 (1/2/3): ";
        string tok;
        if (!(cin >> tok)) return 3;
        if (tok == "1") return 1;
        if (tok == "2") return 2;
        if (tok == "3") return 3;
        cout << "无效选择，请重新输入。\n";
    }
}

static bool inputPolynomialFlow(Polynomial& poly, const string& name, bool& hasTarget) {
    while (true) {
        // 如果已有保存项，先询问是否替换
        if (hasTarget) {
            cout << "检测到当前已存在多项式 " << name << "(x) = ";
            poly.print();
            cout << "\n是否替换该多项式？(1=替换, 0=取消本次输入并返回): ";
            string rep;
            if (!(cin >> rep)) { clearStdin(); return false; }
            if (rep == "0") {
                cout << "取消替换，返回上一级。\n";
                return false;
            }
            else if (rep != "1") {
                cout << "输入无效，默认取消。\n";
                return false;
            }
            // 若选择替换，继续下面流程
        }

        cout << "\n---- 输入多项式 " << name << " ----\n";
        cout << "请输入最高次数 n（非负整数），输入 -1 取消并返回上一级： ";
        int n;
        if (!readDegree(n)) { clearStdin(); return false; }
        if (n == -1) { cout << "已取消，返回上一级。\n"; return false; }
        if (n < 0) { cout << "次数不能为负，请重新输入。\n"; continue; }

        vector<double> coeffs = inputPolynomialCoeffsInteractive(n);
        if (coeffs.empty()) {
            int opt = offerCancelOptionsForAorB(name);
            if (opt == 1) {
                // 重新输入当前（循环继续）
                continue;
            }
            else if (opt == 2) {
                // caller should switch to input other poly
                return false;
            }
            else {
                return false;
            }
        }

        Polynomial tmp(coeffs);
        cout << "你刚输入的 " << name << "(x) = ";
        tmp.print();
        cout << "\n确认保存？(1=保存, 0=撤回并选择其他操作): ";
        string tok;
        if (!(cin >> tok)) { clearStdin(); return false; }
        if (tok == "1") {
            poly.setCoeffs(coeffs);
            hasTarget = true;
            cout << "已保存多项式 " << name << ".\n";
            return true;
        }
        else {
            int opt = offerCancelOptionsForAorB(name);
            if (opt == 1) {
                continue;
            }
            else if (opt == 2) {
                return false;
            }
            else {
                return false;
            }
        }
    }
}

void Menu::show() {
    Polynomial A, B;
    bool hasA = false, hasB = false;

    while (true) {
        cout << "\n===== 多项式计算器 主菜单 =====\n";
        cout << "1. 输入多项式\n";
        cout << "2. 运算（加/减/乘/除）\n";
        cout << "3. 在 x 处求值\n";
        cout << "0. 退出\n";
        cout << "请选择: ";
        string choice;
        if (!(cin >> choice)) { clearStdin(); break; }

        if (choice == "0") break;

        if (choice == "1") {
            while (true) {
                cout << "\n--- 输入多项式子菜单 ---\n";
                cout << "1. 输入多项式 a\n";
                cout << "2. 输入多项式 b\n";
                cout << "0. 返回主菜单\n";
                cout << "请选择: ";
                string sub;
                if (!(cin >> sub)) { clearStdin(); break; }
                if (sub == "0") break;
                if (sub == "1") {
                    bool res = inputPolynomialFlow(A, "a", hasA);
                    // res true 表示已成功保存 a； false 表示用户取消或切换
                    // 如果取消但用户想输入 b，则 loop 回到子菜单继续选择即可
                }
                else if (sub == "2") {
                    bool res = inputPolynomialFlow(B, "b", hasB);
                }
                else {
                    cout << "无效选择，请重试。\n";
                }
                cout << "是否继续输入多项式？(1: 继续输入 a/b, 0: 返回主菜单): ";
                string cont;
                if (!(cin >> cont)) { clearStdin(); break; }
                if (cont == "0") break;
            }
        }
        else if (choice == "2") {
            if (!hasA || !hasB) {
                cout << "请先输入多项式 a 和 b（选择 1 -> 输入多项式）。\n";
                continue;
            }
            while (true) {
                cout << "\n--- 运算子菜单 ---\n";
                cout << "1. a + b\n";
                cout << "2. a - b\n";
                cout << "3. a * b\n";
                cout << "4. a / b （长除法）\n";
                cout << "5. 显示 a 和 b\n";
                cout << "0. 返回主菜单\n";
                cout << "请选择: ";
                string op;
                if (!(cin >> op)) { clearStdin(); break; }
                if (op == "0") break;
                if (op == "1") {
                    Polynomial res = A.add(B);
                    cout << "a + b = " << res.toString() << "\n";
                }
                else if (op == "2") {
                    Polynomial res = A.sub(B);
                    cout << "a - b = " << res.toString() << "\n";
                }
                else if (op == "3") {
                    Polynomial res = A.mul(B);
                    cout << "a * b = " << res.toString() << "\n";
                }
                else if (op == "4") {
                    try {
                        auto pr = A.div(B);
                        cout << "商 = " << pr.first.toString() << "\n";
                        cout << "余数 = " << pr.second.toString() << "\n";
                    }
                    catch (exception& e) {
                        cout << "错误: " << e.what() << "\n";
                    }
                }
                else if (op == "5") {
                    cout << "a(x) = "; A.print(); cout << "\n";
                    cout << "b(x) = "; B.print(); cout << "\n";
                }
                else {
                    cout << "无效选项。\n";
                }
            }
        }
        else if (choice == "3") {
            if (!hasA && !hasB) {
                cout << "尚未输入任何多项式，请先输入 a 或 b。\n";
                continue;
            }
            cout << "\n--- 求值子菜单 ---\n";
            cout << "1. 计算 a(x)\n";
            cout << "2. 计算 b(x)\n";
            cout << "3. 两者都计算\n";
            cout << "0. 返回主菜单\n";
            cout << "请选择: ";
            string s;
            if (!(cin >> s)) { clearStdin(); continue; }
            if (s == "0") continue;
            if ((s == "1" && !hasA) || (s == "2" && !hasB)) {
                cout << "所选多项式尚未输入。\n";
                continue;
            }
            cout << "请输入 x 的值（支持小数）：";
            double x;
            while (!(cin >> x)) {
                cout << "输入无效，请输入数字：";
                clearStdin();
            }
            if (s == "1") {
                double v = A.evaluate(x);
                cout << "a(" << x << ") = " << Polynomial::formatNumber(v) << "\n";
            }
            else if (s == "2") {
                double v = B.evaluate(x);
                cout << "b(" << x << ") = " << Polynomial::formatNumber(v) << "\n";
            }
            else if (s == "3") {
                if (hasA) {
                    double v = A.evaluate(x);
                    cout << "a(" << x << ") = " << Polynomial::formatNumber(v) << "\n";
                }
                if (hasB) {
                    double v = B.evaluate(x);
                    cout << "b(" << x << ") = " << Polynomial::formatNumber(v) << "\n";
                }
            }
            else {
                cout << "无效选项。\n";
            }
        }
        else {
            cout << "无效选项，请重新选择。\n";
        }
    }

    cout << "退出程序。再见！\n";
}
