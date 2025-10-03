// Menu.cpp (���°�)
// Ҫ�����֮ǰ������ Polynomial.hpp / Polynomial.cpp ���루C++17��

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
        // �����û������ a �� b�����˵�ֻ��ʾ���������ʽ����һѡ��Լ��˳���
        if (!hasA || !hasB) {
            cout << "\n===== ����ʽ������ ���������������ʽ a �� b�� =====\n";
            cout << "1. �������ʽ\n";
            cout << "0. �˳�\n";
            cout << "��ѡ��: ";

            int choice;
            if (!(cin >> choice)) { clearStdin(); cout << "������Ч������������ѡ�\n"; continue; }

            if (choice == 0) break;
            if (choice == 1) {
                // �����Ӳ˵���ѡ������ a �� b������ȷ��ʾ��������
                while (true) {
                    cout << "\n--- �����Ӳ˵� ---\n";
                    cout << "1. �������ʽ a\n";
                    cout << "2. �������ʽ b\n";
                    cout << "0. ������һ��\n";
                    cout << "��ѡ��: ";
                    int ic;
                    if (!(cin >> ic)) { clearStdin(); cout << "������Ч��\n"; continue; }
                    if (ic == 0) break;
                    if (ic != 1 && ic != 2) { cout << "��Чѡ��\n"; continue; }

                    Polynomial* p = (ic == 1 ? &A : &B);

                    // ѡ�����뷽ʽ
                    while (true) {
                        cout << "\n��ѡ�����뷽ʽ:\n";
                        cout << "1. �Ƽ������������룺��������ߴ�����Ȼ��Ӹߵ�������ϵ����\n";
                        cout << "2. �������루n c1 e1 c2 e2 ...��\n";
                        cout << "0. ȡ��������\n";
                        cout << "��ѡ��: ";
                        int im;
                        if (!(cin >> im)) { clearStdin(); cout << "������Ч��\n"; continue; }
                        if (im == 0) break;
                        if (im == 1) {
                            // **�ص�**���������ȷ��ʾ�����������ʽ����ߴ��� n������ Polynomial::inputByDegree ��Ҳ����ʾ
                            cout << "\n��ѡ���˰��������루�Ƽ������밴��ʾ���롣\n";
                            p->inputByDegree(cin); // �ú�������ʾ����������ߴ��� n������ʾ�����ζ�ȡϵ��
                            break;
                        }
                        else if (im == 2) {
                            cout << "\n��ѡ�����������루���ݾɸ�ʽ�����밴��ʾ���롣\n";
                            p->inputBySequence(cin);
                            break;
                        }
                        else {
                            cout << "��Чѡ���������������뷽ʽ��\n";
                        }
                    } // ���� ���뷽ʽѭ��

                    if (ic == 1) hasA = true; else hasB = true;
                    cout << (ic == 1 ? "��¼�����ʽ a��\n" : "��¼�����ʽ b��\n");

                    // ���û������Ƿ�����������Ӳ˵��в��������Լ���������һ������ʽ�򷵻أ�
                    cout << "\n�Ƿ�����������Ӳ˵���(1: ����, 0: ������һ��): ";
                    int cont;
                    if (!(cin >> cont)) { clearStdin(); cont = 0; }
                    if (cont == 0) break;
                } // ���� �����Ӳ˵�ѭ��
            }
            else {
                cout << "��Чѡ�������ԡ�\n";
            }
            continue; // �ص������ѭ��
        }

        // ����Ѿ������� a �� b����ʾ�������˵�
        cout << "\n===== ����ʽ������ ���˵� =====\n";
        cout << "1. �������� a �� b\n";
        cout << "2. �������ʽ������ / ��ѧ��ʽ��\n";
        cout << "3. a + b\n";
        cout << "4. a - b\n";
        cout << "5. a * b\n";
        cout << "6. a / b����ϵ�������� -> ���� �� �� ������\n";
        cout << "7. �� x ����ֵ\n";
        cout << "0. �˳�\n";
        cout << "��ѡ��: ";

        int choice2;
        if (!(cin >> choice2)) { clearStdin(); cout << "������Ч������������ѡ�\n"; continue; }

        if (choice2 == 0) break;

        switch (choice2) {
        case 1: {
            // ���������Ӳ˵�����֮ǰ���ƣ�����ࣩ
            while (true) {
                cout << "\n--- ���������Ӳ˵� ---\n";
                cout << "1. �����������ʽ a\n";
                cout << "2. �����������ʽ b\n";
                cout << "0. ������һ��\n";
                cout << "��ѡ��: ";
                int r;
                if (!(cin >> r)) { clearStdin(); cout << "������Ч��\n"; continue; }
                if (r == 0) break;
                if (r != 1 && r != 2) { cout << "��Чѡ��\n"; continue; }
                Polynomial* p = (r == 1 ? &A : &B);
                cout << "��ѡ�����뷽ʽ: 1=����������(�Ƽ�)  2=��������  0=ȡ��\n";
                int im;
                if (!(cin >> im)) { clearStdin(); cout << "������Ч��\n"; continue; }
                if (im == 0) continue;
                if (im == 1) p->inputByDegree(cin);
                else p->inputBySequence(cin);
                if (r == 1) hasA = true; else hasB = true;
                cout << (r == 1 ? "�Ѹ��¶���ʽ a��\n" : "�Ѹ��¶���ʽ b��\n");
            }
            break;
        }
        case 2: {
            // ����Ӳ˵�
            while (true) {
                cout << "\n--- ����Ӳ˵� ---\n";
                cout << "1. �������и�ʽ�����n c1 e1 ...��\n";
                cout << "2. ����ɶ���ѧ��ʽ��������� 3x^4 - x + 2��\n";
                cout << "0. ������һ��\n";
                cout << "��ѡ��: ";
                int oc;
                if (!(cin >> oc)) { clearStdin(); cout << "������Ч��\n"; continue; }
                if (oc == 0) break;
                cout << "��Ҫ�����һ���� 1 -> a, 2 -> b, 3 -> ���������, 0 -> ����: ";
                int which;
                if (!(cin >> which)) { clearStdin(); cout << "������Ч��\n"; continue; }
                if (which == 0) continue;
                if ((which == 1 || which == 3) && !hasA) { cout << "��δ�������ʽ a��\n"; continue; }
                if ((which == 2 || which == 3) && !hasB) { cout << "��δ�������ʽ b��\n"; continue; }
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
                auto pr = A.divide(B); // pair<��, ����>
                cout << "�� = "; pr.first.printPretty();
                cout << "���� = "; pr.second.printPretty();
                cout << "(ע����ǰΪ��ϵ������������ĳ���޷��������������ڸò�ֹͣ�����ص�ǰ������)\n";
            }
            catch (exception& e) {
                cout << "����: " << e.what() << "\n";
            }
            break;
        }
        case 7: {
            cout << "\n��ֵ�˵���\n";
            cout << "1. �� a(x)\n2. �� b(x)\n3. ����\n0. ����\n��ѡ��: ";
            int vc;
            if (!(cin >> vc)) { clearStdin(); cout << "������Ч��\n"; continue; }
            if (vc == 0) break;
            long long x;
            cout << "������ x ������ֵ: ";
            while (!(cin >> x)) { clearStdin(); cout << "������Ч������������ x: "; }
            if ((vc == 1 || vc == 3)) cout << "a(" << x << ") = " << A.evaluate(x) << "\n";
            if ((vc == 2 || vc == 3)) cout << "b(" << x << ") = " << B.evaluate(x) << "\n";
            break;
        }
        default:
            cout << "��Чѡ������ԡ�\n";
        } // end switch
    } // end main while

    cout << "���˳������ټ�~\n";
}
