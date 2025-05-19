#include <iostream>
#include <cmath>
#include <locale>

int segments = 10;

double simpson_rule(double (*f)(double), double a, double b, int n) {
    if (n % 2 == 1) n++; // Делаем n четным
    double h = (b - a) / n;
    double sum = f(a) + f(b);

    for (int i = 1; i < n; i += 2)
        sum += 4 * f(a + i * h);

    for (int i = 2; i < n - 1; i += 2)
        sum += 2 * f(a + i * h);

    return (h / 3) * sum;
}

double function(double x) {
    return 4 - 3; // Интегрируемая функция
}

int main() {
    setlocale(LC_ALL, "Russian");
    double result = simpson_rule(function, 5, 7, segments); // 5 - ограничение справа, 7 - ограничение слева
    std::cout << "Приближенное значение интеграла: " << result << std::endl; // результат выходит точный, тк прямоугольник
    return 0;
}
