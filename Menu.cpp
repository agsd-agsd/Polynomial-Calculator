// Menu.cpp （已更新：在运算子菜单加入“在 x 处求值”，并保证只输出纯数字）
#include "Menu.hpp"
#include "Polynomial.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cmath>

using namespace std;

static void clearStdin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 读取 double 或 q 取消（返回 codes: 0=ok,1=cancel,-1=invalid/EOF）
static int readDoubleOrCancel(double &outVal) {
    string tok;
    if (!(cin >> tok)) return -1;
    if (tok == "q" || tok == "Q") return 1;
    try {
        outVal = stod(tok);
        return 0;
    } catch (...) {
        return -1;
    }
}

// 读取指数（double），支持 q 取消（返回 codes: 0=ok,1=cancel,-1=invalid/EOF）
static int readExpOrCancel(double &outVal) {
    string tok;
    if (!(cin >> tok)) return -1;
    if (tok == "q" || tok == "Q") return 1;
    try {
        outVal = stod(tok);
        return 0;
    } catch (...) {
        return -1;
    }
}

// 当用户在输入过程中取消或确认撤回时提供的三选项
// 返回值：1=重新输入当前多项式, 2=输入另一个多项式, 3=返回主菜单
static int offerCancelOptionsForAorB(const string& name) {
    while (true) {
        cout << "你选择了撤回/取消。\n";
        cout << "请选择：1. 重新输入 " << name << "  2. 输入多项式 " << (name=="a"?"b":"a") << "  3. 返回主菜单\n";
        cout << "请输入选项 (1/2/3): ";
        string tok;
        if (!(cin >> tok)) return 3;
        if (tok == "1") return 1;
        if (tok == "2") return 2;
        if (tok == "3") return 3;
        cout << "无效选择，请重新输入。\n";
    }
}

// 在输入前若已有保存的多项式，先询问是否替换；返回 true 表示允许继续输入并覆盖，false 表示取消
static bool askReplaceIfExists(const string& name, Polynomial& poly, bool has) {
    if (!has) return true;
    cout << "检测到当前已存在多项式 " << name << "(x) = ";
    poly.print();
    cout << "\n是否替换该多项式？(1=替换, 0=取消本次输入并返回): ";
    string rep;
    if (!(cin >> rep)) { clearStdin(); return false; }
    if (rep == "1") return true;
    return false;
}

// 现在只保留按项输入：先输入项数 m，然后逐项输入 (coeff, exp)
// 支持指数为浮点数（可为负）
// 返回 true 表示成功保存（caller 更新 hasTarget）
static bool inputByItemsFlow(Polynomial& poly, const string& name) {
    while (true) {
        cout << "\n---- 按项输入 " << name << " ----\n";
        cout << "请输入项数 m（正整数），输入 -1 取消并返回上一级： ";
        int m;
        string tok;
        if (!(cin >> tok)) { clearStdin(); return false; }
        try {
            m = stoi(tok);
        } catch (...) {
            cout << "输入无效，请输入整数。\n";
            clearStdin();
            return false;
        }
        if (m == -1) { cout << "已取消，返回上一级。\n"; return false; }
        if (m <= 0) { cout << "项数必须为正整数，请重新输入。\n"; continue; }

        vector<Polynomial::Term> tvec;
        bool cancelled = false;

        for (int i = 1; i <= m; ++i) {
            cout << "第 " << i << " 项：\n";
            double coeff;
            while (true) {
                cout << "  系数 (输入 q 取消): ";
                int rc = readDoubleOrCancel(coeff);
                if (rc == -1) { cout << "输入无效，请重新输入。\n"; clearStdin(); continue; }
                if (rc == 1) { cancelled = true; break; }
                break;
            }
            if (cancelled) break;

            double exp;
            while (true) {
                cout << "  指数（可为小数或负数，例如 -1 或 2.5）（输入 q 取消）： ";
                int rc = readExpOrCancel(exp);
                if (rc == -1) { cout << "输入无效，请重新输入指数。\n"; clearStdin(); continue; }
                if (rc == 1) { cancelled = true; break; }
                break;
            }
            if (cancelled) break;

            if (fabs(coeff) > Polynomial::EPS) tvec.emplace_back(coeff, exp);

            // 显示当前临时多项式
            Polynomial tmp(tvec);
            cout << "当前多项式临时为： " << tmp.toString() << "\n";

            cout << "是否继续添加下一个项？(1=继续, 0=取消并选择操作): ";
            string cont;
            if (!(cin >> cont)) { clearStdin(); cancelled = true; break; }
            if (cont == "1") continue;
            else {
                cancelled = true;
                break;
            }
        }

        if (cancelled) {
            int opt = offerCancelOptionsForAorB(name);
            if (opt == 1) {
                continue; // 重新输入当前多项式（从头）
            } else if (opt == 2) {
                return false; // caller 处理切换
            } else {
                return false;
            }
        }

        // 确认保存
        Polynomial tmp(tvec);
        cout << "你刚输入的 " << name << "(x) = ";
        tmp.print();
        cout << "\n确认保存？(1=保存, 0=撤回并选择其他操作): ";
        string tok2;
        if (!(cin >> tok2)) { clearStdin(); return false; }
        if (tok2 == "1") {
            poly.setTerms(tvec);
            cout << "已保存多项式 " << name << ".\n";
            return true;
        } else {
            int opt = offerCancelOptionsForAorB(name);
            if (opt == 1) {
                continue;
            } else if (opt == 2) {
                return false;
            } else {
                return false;
            }
        }
    }
}

// 综合输入流程：询问是否覆盖已有，然后只提供按项输入
static bool inputPolynomialFlow(Polynomial& poly, const string& name, bool& hasTarget) {
    if (hasTarget) {
        bool ok = askReplaceIfExists(name, poly, hasTarget);
        if (!ok) return false;
    }

    bool res = inputByItemsFlow(poly, name);
    if (res) hasTarget = true;
    return res;
}

// 输出风格选择：返回 1 = 序列输出, 2 = 人类可读输出, 0 = 返回/取消
static int chooseOutputStyle() {
    while (true) {
        cout << "\n请选择输出形式：\n";
        cout << "1. 按序列输出（n c1 e1 c2 e2 ...）\n";
        cout << "2. 按人类阅读习惯输出（例如 3x^(2) - x^(-1) + 5）\n";
        cout << "0. 返回（不输出）\n";
        cout << "请选择: ";
        string s;
        if (!(cin >> s)) { clearStdin(); return 0; }
        if (s == "0") return 0;
        if (s == "1") return 1;
        if (s == "2") return 2;
        cout << "无效选择，请重试。\n";
    }
}

void Menu::show() {
    Polynomial A, B;
    bool hasA = false, hasB = false;

    while (true) {
        cout << "\n===== 多项式计算器 主菜单 =====\n";
        cout << "1. 输入多项式\n";
        cout << "2. 运算（加/减/乘/除/显示）\n";
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
                } else if (sub == "2") {
                    bool res = inputPolynomialFlow(B, "b", hasB);
                } else {
                    cout << "无效选择，请重试。\n";
                }
                cout << "是否继续输入多项式？(1: 继续输入 a/b, 0: 返回主菜单): ";
                string cont;
                if (!(cin >> cont)) { clearStdin(); break; }
                if (cont == "0") break;
            }
        } else if (choice == "2") {
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
                cout << "6. 在 x 处求值 (仅在此菜单下，输出纯数字)\n";
                cout << "7. 按序列输出（n c1 e1 c2 e2 ...）\n";
                cout << "0. 返回主菜单\n";
                cout << "请选择: ";
                string op;
                if (!(cin >> op)) { clearStdin(); break; }
                if (op == "0") break;

                if (op == "1") {
                    Polynomial res = A.add(B);
                    int style = chooseOutputStyle();
                    if (style == 1) cout << res.toSequence() << "\n";
                    else if (style == 2) cout << res.toString() << "\n";
                } else if (op == "2") {
                    Polynomial res = A.sub(B);
                    int style = chooseOutputStyle();
                    if (style == 1) cout << res.toSequence() << "\n";
                    else if (style == 2) cout << res.toString() << "\n";
                } else if (op == "3") {
                    Polynomial res = A.mul(B);
                    int style = chooseOutputStyle();
                    if (style == 1) cout << res.toSequence() << "\n";
                    else if (style == 2) cout << res.toString() << "\n";
                } else if (op == "4") {
                    try {
                        auto pr = A.div(B);
                        int style = chooseOutputStyle();
                        if (style == 1) {
                            cout << "商 " << pr.first.toSequence() << "\n";
                            cout << "余数 " << pr.second.toSequence() << "\n";
                        } else if (style == 2) {
                            cout << "商 = " << pr.first.toString() << "\n";
                            cout << "余数 = " << pr.second.toString() << "\n";
                        }
                    } catch (exception& e) {
                        cout << "错误: " << e.what() << "\n";
                    }
                } else if (op == "5") {
                    // 显示 a 和 b: 选择输出样式后输出
                    cout << "选择要显示的对象：1. a  2. b  3. a 和 b  0. 返回\n";
                    cout << "请选择: ";
                    string sel;
                    if (!(cin >> sel)) { clearStdin(); break; }
                    if (sel == "0") continue;
                    int style = chooseOutputStyle();
                    if (style == 0) continue;
                    if (sel == "1") {
                        if (style == 1) cout << "a: " << A.toSequence() << "\n";
                        else cout << "a(x) = " << A.toString() << "\n";
                    } else if (sel == "2") {
                        if (style == 1) cout << "b: " << B.toSequence() << "\n";
                        else cout << "b(x) = " << B.toString() << "\n";
                    } else if (sel == "3") {
                        if (style == 1) {
                            cout << "a: " << A.toSequence() << "\n";
                            cout << "b: " << B.toSequence() << "\n";
                        } else {
                            cout << "a(x) = " << A.toString() << "\n";
                            cout << "b(x) = " << B.toString() << "\n";
                        }
                    } else {
                        cout << "无效选择。\n";
                    }
                } else if (op == "6") {
                    // 在 x 处求值（只输出纯数字）
                    cout << "请选择要计算的对象：1. a  2. b  3. a 和 b  0. 返回\n";
                    cout << "请选择: ";
                    string sel;
                    if (!(cin >> sel)) { clearStdin(); break; }
                    if (sel == "0") continue;
                    // 检查所选是否存在
                    if ((sel == "1" && !hasA) || (sel == "2" && !hasB) || (sel == "3" && !hasA && !hasB)) {
                        cout << "所选多项式尚未输入。\n";
                        continue;
                    }
                    cout << "请输入 x 的值（支持小数）：";
                    double x;
                    while (!(cin >> x)) {
                        cout << "输入无效，请输入数字：";
                        clearStdin();
                    }
                    try {
                        if (sel == "1") {
                            double v = A.evaluate(x);
                            cout << Polynomial::formatNumber(v) << "\n";
                        } else if (sel == "2") {
                            double v = B.evaluate(x);
                            cout << Polynomial::formatNumber(v) << "\n";
                        } else if (sel == "3") {
                            if (hasA) {
                                double va = A.evaluate(x);
                                cout << Polynomial::formatNumber(va) << "\n";
                            }
                            if (hasB) {
                                double vb = B.evaluate(x);
                                cout << Polynomial::formatNumber(vb) << "\n";
                            }
                        } else {
                            cout << "无效选择。\n";
                        }
                    } catch (exception& e) {
                        cout << "错误（求值失败）: " << e.what() << "\n";
                    }
                } else if (op == "7") {
                    // 直接按序列输出（快捷入口）
                    cout << "选择序列输出对象：1. a  2. b  3. a 和 b  0. 返回\n";
                    cout << "请选择: ";
                    string sel;
                    if (!(cin >> sel)) { clearStdin(); break; }
                    if (sel == "0") continue;
                    if (sel == "1") {
                        cout << "a 序列输出: " << A.toSequence() << "\n";
                    } else if (sel == "2") {
                        cout << "b 序列输出: " << B.toSequence() << "\n";
                    } else if (sel == "3") {
                        cout << "a 序列输出: " << A.toSequence() << "\n";
                        cout << "b 序列输出: " << B.toSequence() << "\n";
                    } else {
                        cout << "无效选择。\n";
                    }
                } else {
                    cout << "无效选项。\n";
                }
            }
        } else if (choice == "3") {
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
            try {
                if (s == "1") {
                    double v = A.evaluate(x);
                    cout << "a(" << Polynomial::formatNumber(x) << ") = " << Polynomial::formatNumber(v) << "\n";
                } else if (s == "2") {
                    double v = B.evaluate(x);
                    cout << "b(" << Polynomial::formatNumber(x) << ") = " << Polynomial::formatNumber(v) << "\n";
                } else if (s == "3") {
                    if (hasA) {
                        double v = A.evaluate(x);
                        cout << "a(" << Polynomial::formatNumber(x) << ") = " << Polynomial::formatNumber(v) << "\n";
                    }
                    if (hasB) {
                        double v = B.evaluate(x);
                        cout << "b(" << Polynomial::formatNumber(x) << ") = " << Polynomial::formatNumber(v) << "\n";
                    }
                } else {
                    cout << "无效选项。\n";
                }
            } catch (exception& e) {
                cout << "错误（求值失败）: " << e.what() << "\n";
            }
        } else {
            cout << "无效选项，请重新选择。\n";
        }
    }

    cout << "退出程序。再见！\n";
}
