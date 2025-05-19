#include <iostream>
#include <locale>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;

std::vector<int> generate_arr(int length, int min_val = 0, int max_val = 100) { //ф-ция для генерации массива рандомных чисел от 1 да 10, заданной длины
    std::vector<int> arr(length);
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr))); // seed с текущим временем
    std::uniform_int_distribution<int> dist(min_val, max_val); // диапазон значений

    for (auto& num : arr) {
        num = dist(gen);
    }

    return arr;
}


//std::vector<int> arr = { 9, 3, 7, 4, 1, 5, 6, 2, 8, 10 };

//сортировка вставками
void insert_sort(std::vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) { //сортировка не работала для многозначных чисел и нуля, пришлось подправить ( заменила arr.size()-1 на arr.size() )
        int current = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > current) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = current;
    }
}

//поразрядная сортировка
std::vector<int> combine_baskets(std::vector<int> baskets[10]) {
    std::vector<int> combined_arr;
    for (int basket = 0; basket < 10; basket++) {
        combined_arr.insert(combined_arr.end(), baskets[basket].begin(), baskets[basket].end());
    }
    return combined_arr;
}

void sort_by_rank(std::vector<int>& arr, int radix) {
    std::vector<int> baskets[10];
    for (int i = 0; i < arr.size(); i++) {
        int index_basket = (arr[i] / radix) % 10;
        baskets[index_basket].push_back(arr[i]);
    }
    arr = combine_baskets(baskets);
}

void radix_sort(std::vector<int>& arr) {
    int max_numb = *std::max_element(arr.begin(), arr.end());
    int radix = 1;
    while (max_numb / radix > 0) {
        sort_by_rank(arr, radix);
        radix = radix * 10;
    }
}
//быстрая сортировка

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

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...); // Передаём все аргументы
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
}

//template <typename... Args>
//auto measure_execution_time(void (*func)(Args...), Args&&... args) {
//    auto start_time = std::chrono::high_resolution_clock::now();
//    func(std::forward<Args>(args)...); // не сохраняем результат функции, так как она возвращает void
//    auto end_time = std::chrono::high_resolution_clock::now();
//    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
//}

int main() {
    setlocale(LC_ALL, "Russian");
    auto arr = generate_arr(150);
    //insert_sort(arr);
    //radix_sort(arr);
    //fast_sort(arr, 0, arr.size() - 1);
    /*for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    };*/
    /*auto time_taken = measure_execution_time(insert_sort, arr);
    std::cout << "Время выполнения: " << time_taken << " мcс\n";*/
    /*auto time_taken1 = measure_execution_time(radix_sort, arr);
    std::cout << "Время выполнения: " << time_taken1 << " мcс\n";*/
    auto time_taken2 = measure_execution_time(fast_sort, arr, 0, static_cast<int>(arr.size()) - 1);
    std::cout << "Время выполнения: " << time_taken2 << " мcс\n";
    return 0;
}