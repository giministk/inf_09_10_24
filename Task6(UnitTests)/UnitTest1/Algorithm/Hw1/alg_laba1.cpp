#include <iostream>
#include <locale>
#include <vector>
#include <chrono>

const int length = 10;
const int length1 = 100;
const int length2 = 1000;
const int length3 = 10000;
const int length4 = 100000;
const int length5 = 1000000;
const int length6 = 3000000;
const int length7 = 3000;
const int length8 = 30000;
const int length9 = 300000;

//std::vector<int> arr = { 1, 5, 3, 9, 2 };
std::vector<int> arr;
int target = 3;

int linear_search(const std::vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();  // Начинаем отсчет времени
    auto result = func(std::forward<Args>(args)...);  // Выполняем переданную функцию с аргументами
    auto end_time = std::chrono::high_resolution_clock::now();  // Останавливаем отсчет времени

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);  // Вычисляем продолжительность в миллисекундах
    return std::make_pair(result, duration.count());  // Возвращаем результат и время выполнения
}



int main() {
    setlocale(LC_ALL, "Russian");
    for (int i = 0; i < length6 - 1; ++i) {
        arr.push_back(0);
    }
    arr.push_back(3);
    auto [result, execution_time] = measure_execution_time(linear_search, arr, target);
    std::cout << "Результат: " << result << ", Время выполнения: " << execution_time << " мcс" << std::endl;
    return 0;
}

//int main() {
//    std::vector<int> arr = { 1, 5, 3, 9, 2 };
//    int target = 3;
//    int index = linear_search(arr, target);
//
//    if (index != -1)
//        std::cout << "Element found at index: " << index << std::endl;
//    else
//        std::cout << "Element not found" << std::endl;
//
//    return 0;
//}