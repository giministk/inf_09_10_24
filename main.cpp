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

    int N; // Размер массивов

    // Ввод размера массивов
    std::cout << "Введите размер массива (N): ";
    std::cin >> N;

    int* A = new int[N]; // Динамическое выделение памяти для массива A
    
    // Ввод массивов
    std::cout << "Введите " << N << " элементов для массива A:" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cin >> A[i];
    }

    // функция 1: нечётные
    newArr(A, N);

    // Освобождение выделенной памяти
    delete[] A;

    return 0;
}
