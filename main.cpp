#include <iostream>

int* newArr(const int* arr, int size) {
    int n = 0;
    int* arr2 = new int[size];
    for (int i = 0; i < size; i++) {
        if (arr[i] % 2 != 0) {
            arr2[n] = arr[i];
            std::cout << arr2[n] << ' ';
            n += 1;
        }
    }
    std::cout << std::endl;
    return arr2;
}


int main() {
    setlocale(LC_ALL, "Russian");

    int N; // ������ ��������

    // ���� ������� ��������
    std::cout << "������� ������ ������� (N): ";
    std::cin >> N;

    int* A = new int[N]; // ������������ ��������� ������ ��� ������� A
    
    // ���� ��������
    std::cout << "������� " << N << " ��������� ��� ������� A:" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cin >> A[i];
    }

    // ������� 1: ��������
    newArr(A, N);

    // ������������ ���������� ������
    delete[] A;

    return 0;
}
