#include <stdio.h>

#include "result/result.h"
#include "result/error.h"

/**
 * @brief Result type with project custom error type.
 */
template<class T>
using Result = mxc::Result<T, mxc::Error>;
using Error = mxc::Error;
using mxc::err, mxc::ok;

/**
 * @brief Safe division function.
 */
Result<f32> safe_div(f32 a, f32 b) {
    if (b == 0.0f) {
        return err(Error("Divide by zero", TRACE));
    }

    return ok(a / b);
}

/**
 * @brief Result without a return value.
 */
Result<void> something_stupid() {
    return err(Error("Something stupid happened", TRACE));
}

Result<f32> try_multiple() { 
    f32 r1 = safe_div(10, 5).unwrap();
    f32 r2 = safe_div(20, 5).unwrap();
    return safe_div(r1, r2);
}

int main() {
    printf("Hello, world!\n");

    Result<void> r = something_stupid();
    if (r.is_err()) {
        const Error& e = r.unwrap_err();
        if (e.trace.file_name != nullptr) {
            printf("[error] (%s:%zi) %s\n", e.trace.file_name, e.trace.line_num, e.info);
        } else {
            printf("[error] %s\n", e.info);
        }
        return 0;
    }

    try_multiple();

    printf("16 / 4 = %f\n", safe_div(16, 4).expect("failed to divide"));
    printf("20 / 0 = %f\n", safe_div(20, 0).unwrap_or(0.0f));


    printf("Bye, world!\n");

    return 0;
}
