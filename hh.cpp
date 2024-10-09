
#include <iostream>
#include <locale>
#include <cmath>



int main(){

    setlocale(LC_ALL, "Russian");

    float x, y, c, r, s;

    std::cin >> x;
    std::cin >> y;

    r = pow(pow(y, 2) + pow(x, 2), (1/2)) / x;
    s = pow(cos((asin(1.0) / 2) * x), 2);

    std::cout << r << std::endl;
    std::cout << s << std::endl;

    if (r < s) {
        c = s;
    }
    else {
        c = r;
    }

    std::cout << c << std::endl;

    return 0;
};

