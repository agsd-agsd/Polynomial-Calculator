// Menu.cpp (更新版)
// 要求：配合之前给出的 Polynomial.hpp / Polynomial.cpp 编译（C++17）

#include "Menu.hpp"
#include "Polynomial.hpp"
#include <iostream>
#include <limits>

using namespace std;

static void clearStdin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Menu::show() {
    Polynomial A, B;
    bool hasA = false, hasB = false;

    while (true) {
        // 如果还没输入完 a 和 b，主菜单只显示“输入多项式”这一选项（以及退出）
        if (!hasA || !hasB) {
            cout << "\n===== 多项式计算器 （首先请输入多项式 a 与 b） =====\n";
            cout << "1. 输入多项式\n";
            cout << "0. 退出\n";
            cout << "请选择: ";

            int choice;
            if (!(cin >> choice)) { clearStdin(); cout << "输入无效，请输入数字选项。\n"; continue; }

            if (choice == 0) break;
            if (choice == 1) {
                // 输入子菜单：选择输入 a 或 b，并明确提示次数输入
                while (true) {
                    cout << "\n--- 输入子菜单 ---\n";
                    cout << "1. 输入多项式 a\n";
                    cout << "2. 输入多项式 b\n";
                    cout << "0. 返回上一级\n";
                    cout << "请选择: ";
                    int ic;
                    if (!(cin >> ic)) { clearStdin(); cout << "输入无效。\n"; continue; }
                    if (ic == 0) break;
                    if (ic != 1 && ic != 2) { cout << "无效选择。\n"; continue; }

                    Polynomial* p = (ic == 1 ? &A : &B);

                    // 选择输入方式
                    while (true) {
                        cout << "\n请选择输入方式:\n";
                        cout << "1. 推荐（按次数输入：先输入最高次数，然后从高到低输入系数）\n";
                        cout << "2. 序列输入（n c1 e1 c2 e2 ...）\n";
                        cout << "0. 取消并返回\n";
                        cout << "请选择: ";
                        int im;
                        if (!(cin >> im)) { clearStdin(); cout << "输入无效。\n"; continue; }
                        if (im == 0) break;
                        if (im == 1) {
                            // **重点**：这里会明确提示“请输入多项式的最高次数 n”，在 Polynomial::inputByDegree 中也有提示
                            cout << "\n你选择了按次数输入（推荐）。请按提示输入。\n";
                            p->inputByDegree(cin); // 该函数会显示“请输入最高次数 n”的提示并依次读取系数
                            break;
                        }
                        else if (im == 2) {
                            cout << "\n你选择了序列输入（兼容旧格式）。请按提示输入。\n";
                            p->inputBySequence(cin);
                            break;
                        }
                        else {
                            cout << "无效选择，请重新输入输入方式。\n";
                        }
                    } // 结束 输入方式循环

                    if (ic == 1) hasA = true; else hasB = true;
                    cout << (ic == 1 ? "已录入多项式 a。\n" : "已录入多项式 b。\n");

                    // 让用户决定是否继续在输入子菜单中操作（可以继续输入另一个多项式或返回）
                    cout << "\n是否继续在输入子菜单？(1: 继续, 0: 返回上一级): ";
                    int cont;
                    if (!(cin >> cont)) { clearStdin(); cont = 0; }
                    if (cont == 0) break;
                } // 结束 输入子菜单循环
            }
            else {
                cout << "无效选择，请重试。\n";
            }
            continue; // 回到最外层循环
        }

        // 如果已经输入完 a 与 b，显示完整主菜单
        cout << "\n===== 多项式计算器 主菜单 =====\n";
        cout << "1. 重新输入 a 或 b\n";
        cout << "2. 输出多项式（序列 / 数学格式）\n";
        cout << "3. a + b\n";
        cout << "4. a - b\n";
        cout << "5. a * b\n";
        cout << "6. a / b（整系数长除法 -> 返回 商 与 余数）\n";
        cout << "7. 在 x 处求值\n";
        cout << "0. 退出\n";
        cout << "请选择: ";

        int choice2;
        if (!(cin >> choice2)) { clearStdin(); cout << "输入无效，请输入数字选项。\n"; continue; }

        if (choice2 == 0) break;

        switch (choice2) {
        case 1: {
            // 重新输入子菜单（和之前类似，但简洁）
            while (true) {
                cout << "\n--- 重新输入子菜单 ---\n";
                cout << "1. 重新输入多项式 a\n";
                cout << "2. 重新输入多项式 b\n";
                cout << "0. 返回上一级\n";
                cout << "请选择: ";
                int r;
                if (!(cin >> r)) { clearStdin(); cout << "输入无效。\n"; continue; }
                if (r == 0) break;
                if (r != 1 && r != 2) { cout << "无效选择。\n"; continue; }
                Polynomial* p = (r == 1 ? &A : &B);
                cout << "请选择输入方式: 1=按次数输入(推荐)  2=序列输入  0=取消\n";
                int im;
                if (!(cin >> im)) { clearStdin(); cout << "输入无效。\n"; continue; }
                if (im == 0) continue;
                if (im == 1) p->inputByDegree(cin);
                else p->inputBySequence(cin);
                if (r == 1) hasA = true; else hasB = true;
                cout << (r == 1 ? "已更新多项式 a。\n" : "已更新多项式 b。\n");
            }
            break;
        }
        case 2: {
            // 输出子菜单
            while (true) {
                cout << "\n--- 输出子菜单 ---\n";
                cout << "1. 机器序列格式输出（n c1 e1 ...）\n";
                cout << "2. 人类可读数学格式输出（例如 3x^4 - x + 2）\n";
                cout << "0. 返回上一级\n";
                cout << "请选择: ";
                int oc;
                if (!(cin >> oc)) { clearStdin(); cout << "输入无效。\n"; continue; }
                if (oc == 0) break;
                cout << "你要输出哪一个？ 1 -> a, 2 -> b, 3 -> 两个都输出, 0 -> 返回: ";
                int which;
                if (!(cin >> which)) { clearStdin(); cout << "输入无效。\n"; continue; }
                if (which == 0) continue;
                if ((which == 1 || which == 3) && !hasA) { cout << "尚未输入多项式 a。\n"; continue; }
                if ((which == 2 || which == 3) && !hasB) { cout << "尚未输入多项式 b。\n"; continue; }
                if (oc == 1) {
                    if (which == 1 || which == 3) A.printSequence();
                    if (which == 2 || which == 3) B.printSequence();
                }
                else {
                    if (which == 1 || which == 3) A.printPretty();
                    if (which == 2 || which == 3) B.printPretty();
                }
            }
            break;
        }
        case 3: {
            Polynomial res = A.add(B);
            cout << "a + b = "; res.printPretty();
            break;
        }
        case 4: {
            Polynomial res = A.sub(B);
            cout << "a - b = "; res.printPretty();
            break;
        }
        case 5: {
            Polynomial res = A.multiply(B);
            cout << "a * b = "; res.printPretty();
            break;
        }
        case 6: {
            try {
                auto pr = A.divide(B); // pair<商, 余数>
                cout << "商 = "; pr.first.printPretty();
                cout << "余数 = "; pr.second.printPretty();
                cout << "(注：当前为整系数长除法；若某步无法整除，除法将在该步停止并返回当前余数。)\n";
            }
            catch (exception& e) {
                cout << "错误: " << e.what() << "\n";
            }
            break;
        }
        case 7: {
            cout << "\n求值菜单：\n";
            cout << "1. 求 a(x)\n2. 求 b(x)\n3. 都求\n0. 返回\n请选择: ";
            int vc;
            if (!(cin >> vc)) { clearStdin(); cout << "输入无效。\n"; continue; }
            if (vc == 0) break;
            long long x;
            cout << "请输入 x 的整数值: ";
            while (!(cin >> x)) { clearStdin(); cout << "输入无效，请输入整数 x: "; }
            if ((vc == 1 || vc == 3)) cout << "a(" << x << ") = " << A.evaluate(x) << "\n";
            if ((vc == 2 || vc == 3)) cout << "b(" << x << ") = " << B.evaluate(x) << "\n";
            break;
        }
        default:
            cout << "无效选项，请重试。\n";
        } // end switch
    } // end main while

    cout << "已退出程序。再见~\n";
}
