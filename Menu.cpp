// Menu.cpp （按你最新要求改造）
// - 在父菜单里提示输入方式（唯一：按次数输入）并读取最高次数 n（可取消 -1）
// - 输入 a 后询问是否继续输入 b（1: 输入 b, 0: 返回主菜单）
// - 如果替换已有多项式，先提示当前多项式并要求确认
// - 每一步都有撤回/确认机制

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
        // 当尚未同时输入 a 和 b 时，主菜单只显示输入与退出
        if (!hasA || !hasB) {
            cout << "\n===== 多项式计算器（第一步：请输入多项式） =====\n";
            cout << "说明：当前输入方式为【按次数输入】（这是唯一的输入方式）。\n";
            cout << "操作：选择“输入多项式”后，先输入最高次数 n（非负整数），然后按从 x^n 到 x^0 依次输入系数。\n";
            cout << "（注：输入系数时输入 q 可撤回并取消本次多项式输入。）\n";
            cout << "1. 输入多项式\n";
            cout << "0. 退出\n";
            cout << "请选择: ";

            int choice;
            if (!(cin >> choice)) { clearStdin(); cout << "输入无效，请输入数字选项。\n"; continue; }

            if (choice == 0) break;
            if (choice != 1) { cout << "无效选择。\n"; continue; }

            // 父菜单先读最高次数（或允许取消）
            cout << "请输入多项式的最高次数 n（非负整数）。如需取消本次输入，请输入 -1 然后回车: ";
            int presetN;
            while (!(cin >> presetN) || presetN < -1) {
                cout << "输入无效，请输入非负整数 n 或 -1 取消： ";
                clearStdin();
            }
            if (presetN == -1) {
                cout << "已取消，返回主菜单。\n";
                continue;
            }

            // 选择输入哪一个（a 或 b）
            while (true) {
                cout << "\n请选择要输入的多项式：\n";
                cout << "1. 输入多项式 a\n";
                cout << "2. 输入多项式 b\n";
                cout << "0. 取消并返回主菜单\n";
                cout << "请选择: ";
                int which;
                if (!(cin >> which)) { clearStdin(); cout << "输入无效。\n"; continue; }
                if (which == 0) break;
                if (which != 1 && which != 2) { cout << "无效选择。\n"; continue; }

                Polynomial* target = (which == 1 ? &A : &B);
                bool* hasTarget = (which == 1 ? &hasA : &hasB);

                // 如果目标已有多项式，先询问是否替换
                if (*hasTarget) {
                    cout << "\n检测到你将覆盖已存在的多项式 ";
                    cout << (which == 1 ? "a = " : "b = ");
                    target->printPretty();
                    cout << "是否替换该多项式？(1=替换, 0=取消本次输入并返回): ";
                    int rep;
                    while (!(cin >> rep) || (rep != 0 && rep != 1)) {
                        cout << "请输入 1 (替换) 或 0 (取消)： ";
                        clearStdin();
                    }
                    if (rep == 0) {
                        cout << "取消替换，返回到选择 a/b。\n";
                        continue;
                    }
                    else {
                        cout << "将替换原多项式。\n";
                    }
                }

                // 调用按次数输入（使用父菜单读好的 presetN）
                target->inputByDegree(cin, presetN);
                if (target->empty()) {
                    // 用户在输入过程中撤回或取消（inputByDegree 会处理并清空 terms）
                    cout << "本次输入未保存。\n";
                }
                else {
                    *hasTarget = true;
                }

                // 如果刚输入的是 a，询问是否继续输入 b（1: 输入 b，0: 返回主菜单）
                if (which == 1) {
                    cout << "\n是否继续输入多项式 b？(1: 输入 b, 0: 返回主菜单): ";
                    int cont;
                    while (!(cin >> cont) || (cont != 0 && cont != 1)) {
                        cout << "请输入 1 继续输入 b，或 0 返回主菜单： ";
                        clearStdin();
                    }
                    if (cont == 1) {
                        // 直接跳到输入 b（使用相同 presetN? 让用户选择是否使用相同次数）
                        cout << "是否使用相同的最高次数 n = " << presetN << " 来输入 b？(1: 使用, 0: 重新输入次数): ";
                        int useSame;
                        while (!(cin >> useSame) || (useSame != 0 && useSame != 1)) {
                            cout << "请输入 1 使用相同次数，或 0 重新输入次数： ";
                            clearStdin();
                        }
                        if (useSame == 1) {
                            // 输入 b with presetN
                            // 如果已有 b，先询问是否替换
                            if (hasB) {
                                cout << "\n检测到你将覆盖已存在的多项式 b = ";
                                B.printPretty();
                                cout << "是否替换该多项式？(1=替换, 0=取消并返回主菜单): ";
                                int repb;
                                while (!(cin >> repb) || (repb != 0 && repb != 1)) {
                                    cout << "请输入 1 或 0： ";
                                    clearStdin();
                                }
                                if (repb == 0) {
                                    cout << "取消替换，返回主菜单。\n";
                                    break;
                                }
                            }
                            B.inputByDegree(cin, presetN);
                            if (!B.empty()) hasB = true;
                        }
                        else {
                            // 重新要求输入次数（若用户希望不同次数）
                            cout << "请输入多项式 b 的最高次数 n（非负整数）。如需取消请输入 -1: ";
                            int newN;
                            while (!(cin >> newN) || newN < -1) {
                                cout << "输入无效，请输入非负整数 n 或 -1 取消： ";
                                clearStdin();
                            }
                            if (newN == -1) {
                                cout << "已取消，返回主菜单。\n";
                                break;
                            }
                            // 如果已有 b，询问是否替换
                            if (hasB) {
                                cout << "\n检测到你将覆盖已存在的多项式 b = ";
                                B.printPretty();
                                cout << "是否替换该多项式？(1=替换, 0=取消并返回主菜单): ";
                                int repb;
                                while (!(cin >> repb) || (repb != 0 && repb != 1)) {
                                    cout << "请输入 1 或 0： ";
                                    clearStdin();
                                }
                                if (repb == 0) {
                                    cout << "取消替换，返回主菜单。\n";
                                    break;
                                }
                            }
                            B.inputByDegree(cin, newN);
                            if (!B.empty()) hasB = true;
                        }
                        // 输入 b 结束后，返回主菜单
                        break;
                    }
                    else {
                        // cont == 0: 直接返回主菜单
                        break;
                    }
                }
                else {
                    // 如果这次输入的是 b（which==2），输入完成后直接返回主菜单
                    break;
                }
            } // end choose a/b loop

            continue; // 回到主 while
        } // end not both A&B

        // 当 hasA && hasB 为 true，显示完整主菜单
        cout << "\n===== 多项式计算器 主菜单 =====\n";
        cout << "1. 重新输入 a 或 b\n";
        cout << "2. 输出多项式（序列 / 手写数学格式）\n";
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
            // 重新输入 a 或 b（与之前相似，但简洁）
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
                bool* hasP = (r == 1 ? &hasA : &hasB);

                cout << "请输入新的最高次数 n（非负整数），或 -1 取消: ";
                int newN;
                while (!(cin >> newN) || newN < -1) {
                    cout << "输入无效，请输入非负整数 n 或 -1 取消： ";
                    clearStdin();
                }
                if (newN == -1) continue;

                // 替换确认
                if (*hasP) {
                    cout << "检测到当前 " << (r == 1 ? "a = " : "b = ");
                    p->printPretty();
                    cout << "是否替换？(1=替换, 0=取消): ";
                    int rep;
                    while (!(cin >> rep) || (rep != 0 && rep != 1)) {
                        cout << "请输入 1 或 0： ";
                        clearStdin();
                    }
                    if (rep == 0) { cout << "取消替换。\n"; continue; }
                }
                p->inputByDegree(cin, newN);
                if (!p->empty()) *hasP = true;
                break;
            }
            break;
        }
        case 2: {
            // 输出子菜单
            while (true) {
                cout << "\n--- 输出子菜单 ---\n";
                cout << "1. 机器序列格式输出（n c1 e1 ...）\n";
                cout << "2. 手写数学格式输出（例如 3x² - x + 2）\n";
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
