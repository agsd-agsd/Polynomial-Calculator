#include "Menu.hpp"
#include <iostream>
using namespace std;

void Menu::show() {
    Polynomial a, b;
    bool hasInput = false;

    while (true) {
        cout << "\n===== 多项式计算器 =====\n";
        cout << "1. 输入多项式 a 和 b\n";
        cout << "2. 输出多项式 a 和 b\n";
        cout << "3. a + b\n";
        cout << "4. a - b\n";
        cout << "5. a * b\n";
        cout << "6. a / b (商和余数)\n";
        cout << "7. 在 x 处求值\n";
        cout << "0. 退出\n";
        cout << "请选择操作: ";

        int choice;
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1:
            cout << "请输入多项式 a（格式：n c1 e1 c2 e2 ...）:" << endl;
            a.input();
            cout << "请输入多项式 b:" << endl;
            b.input();
            hasInput = true;
            break;
        case 2:
            if (!hasInput) { cout << "请先输入多项式！\n"; break; }
            cout << "a: "; a.print();
            cout << "b: "; b.print();
            break;
        case 3:
            if (!hasInput) { cout << "请先输入多项式！\n"; break; }
            cout << "a + b = "; a.add(b).print();
            break;
        case 4:
            if (!hasInput) { cout << "请先输入多项式！\n"; break; }
            cout << "a - b = "; a.sub(b).print();
            break;
        case 5:
            if (!hasInput) { cout << "请先输入多项式！\n"; break; }
            cout << "a * b = "; a.multiply(b).print();
            break;
        case 6:
            if (!hasInput) { cout << "请先输入多项式！\n"; break; }
            try {
                auto [q, r] = a.divide(b);
                cout << "商: "; q.print();
                cout << "余数: "; r.print();
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
            break;
        case 7:
            if (!hasInput) { cout << "请先输入多项式！\n"; break; }
            int x;
            cout << "请输入 x: "; cin >> x;
            cout << "a(" << x << ") = " << a.evaluate(x) << endl;
            cout << "b(" << x << ") = " << b.evaluate(x) << endl;
            break;
        default:
            cout << "无效选择，请重试。\n";
        }
    }
}
