#include <iostream>
#include <cmath>



float TwoFunk(float x, float y) {

    float c, r, s;

    r = pow(pow(y, 2) + pow(x, 2), (1 / 2)) / x;
    s = pow(cos((asin(1.0) / 2) * x), 2);

    if (r < s) {
        c = s;
    }
    else {
        c = r;
    }

    return c;
};

int main() {
    return 0;
}