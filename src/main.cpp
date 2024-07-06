#include <stdio.h>

#include "result/result.h"
#include "result/error.h"

/**
 * @brief Result type with project custom error type.
 */
template<class T>
using product = result<T, error>;

/**
 * @brief Safe division function.
 */
product<f32> safe_div(f32 a, f32 b) {
    if (b == 0.0f) {
        return err(error("Divide by zero"));
    }

    return ok(a / b);
}

/**
 * @brief Result without a return value.
 */
product<void> something_stupid() {
    return err(error("Something stupid happened"));
}

product<f32> try_multiple() { 
    f32 r1 = safe_div(10, 5).unwrap();
    f32 r2 = safe_div(20, 5).unwrap();
    return safe_div(r1, r2);
}

int main() {
    printf("Hello, world!\n");

    //something_stupid().unwrap();

    try_multiple();

    printf("16 / 4 = %f\n", safe_div(16, 4).expect("failed to divide"));
    printf("20 / 0 = %f\n", safe_div(20, 0).unwrap_or(0));


    printf("Bye, world!\n");

    return 0;
}
