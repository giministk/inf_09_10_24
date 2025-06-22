#include <iostream>
#include <vector>
#include <climits> 
#include <locale>

using namespace std;

int findMin(const vector<int>& arr, int left, int right) {
    if (left == right) {
        return arr[left];
    }

    if (right - left == 1) {
        return min(arr[left], arr[right]);
    }

    int mid = left + (right - left) / 2;
    int leftMin = findMin(arr, left, mid);
    int rightMin = findMin(arr, mid + 1, right);

    return min(leftMin, rightMin);
}

int findMax(const vector<int>& arr, int left, int right) {
    if (left == right) {
        return arr[left];
    }

    if (right - left == 1) {
        return max(arr[left], arr[right]);
    }

    int mid = left + (right - left) / 2;
    int leftMax = findMax(arr, left, mid);
    int rightMax = findMax(arr, mid + 1, right);

    return max(leftMax, rightMax);
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<int> arr = { 5, 7, 2, 4, 9, 6 };
    int n, num;

    int minVal = findMin(arr, 0, arr.size() - 1);
    int maxVal = findMax(arr, 0, arr.size() - 1);

    cout << "Минимальный элемент: " << minVal << endl;
    cout << "Максимальный элемент: " << maxVal << endl;

    return 0;
}