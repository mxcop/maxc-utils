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

int main() {
    printf("Hello, world!\n");

    //std::vector<int> poop;

    //poop.push_back(12);

    //something_stupid().unwrap();

    printf("16 / 4 = %f\n", safe_div(16, 4).expect("failed to divide"));
    // printf("20 / 0 = %f\n", safe_div(20, 0).unwrap());

    //printf("Start Scope\n");
    //{
    //    result<f32> failed = err("Dangit");
    //    printf("Created error\n");
    //}
    //printf("End Scope\n");

    //{
    //    Box<error> test = Box<error>(new error("Test", "", "", 1));

    //    result<f32> r = safe_div(16, 0);
    //    if (r.is_error()) {
    //        printf("Not allowed!\n");
    //    }
    //    else {
    //        printf("16 / 0 = %f\n", r.unwrap());
    //    }
    //}

    printf("Bye, world!\n");

    return 0;
}
