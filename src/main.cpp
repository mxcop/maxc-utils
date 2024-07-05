#include <stdio.h>

#include "result/result.h"

/**
 * @brief Safe division function.
 */
result<f32> safe_div(f32 a, f32 b) {
    if (b == 0.0f) return err("Attempt to divide by zero");
    return a / b;
}

int main() {
    printf("Hello, world!\n");

    printf("16 / 4 = %f\n", safe_div(16, 4).unwrap());

    printf("Start Scope\n");
    {
        result<f32> failed = err("Dangit");
        printf("Created error\n");
    }
    printf("End Scope\n");

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
