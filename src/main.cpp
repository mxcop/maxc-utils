#include <stdio.h>

#include "result/result.h"

/**
 * @brief Safe division function.
 */
result<float> safe_div(float a, float b) {
    if (b == 0.0f) return err("Attempt to divide by zero");
    return a / b;
}

int main() {
    printf("Hello, world!\n");

    printf("16 / 4 = %f\n", safe_div(16, 4).unwrap());
    printf("16 / 0 = %f\n", safe_div(16, 0).unwrap());

    return 0;
}
