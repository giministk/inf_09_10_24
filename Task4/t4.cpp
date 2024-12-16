#include <iostream>
#include <set>

void get_result(std::set<int> setA, std::set<int> setB) {
    for (int i : setA) {
        for (int n : setB) {
            if (i == n and i > 0) {
                std::cout << i << ' ';
            }
        }
    }
}



int main() {
    setlocale(LC_ALL, "Russian");

    int N_A, N_B, insertnum;
    std::set<int> A, B;

    std::wcout << L"Введите размер множества A: ";
    std::cin >> N_A;

    std::wcout << L"Введите " << N_A << L" элементов для множества A:" << std::endl;
    for (int i = 0; i < N_A; i++) {
        std::cin >> insertnum;
        A.insert(insertnum);
    }

    std::wcout << L"Введите размер множества B: ";
    std::cin >> N_B;

    std::wcout << L"Введите " << N_B << L" элементов для множества B:" << std::endl;
    for (int i = 0; i < N_B; i++) {
        std::cin >> insertnum;
        B.insert(insertnum);
    }

    get_result(A, B);

    std::wcout << L"Нажмите Enter для выхода...";
    std::cin.get();
    std::cin.get();

    return 0;
}