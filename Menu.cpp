#include "Menu.hpp"
#include <iostream>
using namespace std;

void Menu::show() {
    Polynomial a, b;
    bool hasInput = false;

    while (true) {
        cout << "\n===== ����ʽ������ =====\n";
        cout << "1. �������ʽ a �� b\n";
        cout << "2. �������ʽ a �� b\n";
        cout << "3. a + b\n";
        cout << "4. a - b\n";
        cout << "5. a * b\n";
        cout << "6. a / b (�̺�����)\n";
        cout << "7. �� x ����ֵ\n";
        cout << "0. �˳�\n";
        cout << "��ѡ�����: ";

        int choice;
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1:
            cout << "���������ʽ a����ʽ��n c1 e1 c2 e2 ...��:" << endl;
            a.input();
            cout << "���������ʽ b:" << endl;
            b.input();
            hasInput = true;
            break;
        case 2:
            if (!hasInput) { cout << "�����������ʽ��\n"; break; }
            cout << "a: "; a.print();
            cout << "b: "; b.print();
            break;
        case 3:
            if (!hasInput) { cout << "�����������ʽ��\n"; break; }
            cout << "a + b = "; a.add(b).print();
            break;
        case 4:
            if (!hasInput) { cout << "�����������ʽ��\n"; break; }
            cout << "a - b = "; a.sub(b).print();
            break;
        case 5:
            if (!hasInput) { cout << "�����������ʽ��\n"; break; }
            cout << "a * b = "; a.multiply(b).print();
            break;
        case 6:
            if (!hasInput) { cout << "�����������ʽ��\n"; break; }
            try {
                auto [q, r] = a.divide(b);
                cout << "��: "; q.print();
                cout << "����: "; r.print();
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
            break;
        case 7:
            if (!hasInput) { cout << "�����������ʽ��\n"; break; }
            int x;
            cout << "������ x: "; cin >> x;
            cout << "a(" << x << ") = " << a.evaluate(x) << endl;
            cout << "b(" << x << ") = " << b.evaluate(x) << endl;
            break;
        default:
            cout << "��Чѡ�������ԡ�\n";
        }
    }
}
