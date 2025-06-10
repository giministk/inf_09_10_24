#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>

using namespace std;

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...); // Передаём все аргументы
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
}

std::vector<int> generate_arr(int length, int min_val = 0, int max_val = 100) { //ф-ция для генерации массива рандомных чисел от 1 да 10, заданной длины
    std::vector<int> arr(length);
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr))); // seed с текущим временем
    std::uniform_int_distribution<int> dist(min_val, max_val); // диапазон значений

    for (auto& num : arr) {
        num = dist(gen);
    }

    return arr;
}

int devide(std::vector<int>& arr, int bottom, int high) {
    int reference = arr[high];
    int i = bottom - 1;
    for (int j = bottom; j < high; j++) {
        if (arr[j] <= reference) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void fast_sort(std::vector<int>& arr, int bottom, int high) {
    if (bottom < high) {
        int reference_index = devide(arr, bottom, high);
        fast_sort(arr, bottom, reference_index - 1);
        fast_sort(arr, reference_index + 1, high);
    }

}

// Функция быстрой сортировки
void quicksort(vector<int>& arr, int num_threads) {
    vector<thread> thread_mas;

    int pivot = arr.size() / num_threads;  // Опорный элемент (средний)
    vector<int> left, right;

    // Разделяем элементы на две части: меньше и больше опорного
    for (int i = 0; i < num_threads; ++i) {
        int end = (i == num_threads - 1) ? arr.size() - 1 : i * pivot + pivot - 1;
        thread_mas.emplace_back(fast_sort, ref(arr), i * pivot, end);
    }

    for (int i = 0; i < thread_mas.size(); ++i) {
        thread_mas[i].join();
    }
    // Объединяем отсортированные части и возвращаем результат

    int step = pivot;
    while (arr.size() > step) {
        for (int i = 0; i < arr.size() - step; i += 2 * step) {
            int left = i;
            int mid = i + step;
            int right = std::min<int>(i + 2 * step, arr.size());
            inplace_merge(arr.begin() + left, arr.begin() + mid, arr.begin() + right);
        }
        step *= 2;
    }
}

int main() {
    int num_threads = 8;

    //vector<int> arr = { 3, 6, 8, 10, 1, 2, 1 };
    //vector<int> arr = generate_arr(100);
    //
    //quicksort(arr, num_threads);

    // Вывод отсортированного массива
    /*for (int num : arr) {
        cout << num << " ";
    }*/

    auto arr = generate_arr(50000);

    auto time_taken = measure_execution_time(quicksort, arr, num_threads);
    std::cout << "Время выполнения: " << time_taken << " мcс\n";
    return 0;

}