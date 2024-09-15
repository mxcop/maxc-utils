#include <stdio.h>
#include <chrono>

#include "result/single-result.h"

using namespace std::chrono;
using namespace mxc;

template <typename T>
inline bool assert_result(const Result<T>& r) {
    if (r.is_err()) printf("error: %s\n", r.unwrap_err().c_str());
    return r.is_ok();
};

Result<int> test() { return Ok(12); }

Result<int> test_void() {
    return Ok(24);
    // return Err("oh shit");
}
 
Result<void> safe_main() {
    Result<int> rv = test();
    if (rv.is_ok()) {
        printf("rv: %d\n", rv.unwrap());
    }

    const Result<int> r = test_void();
    const int ri = chain_err(r);
    printf("r: %d\n", ri);

    return Ok();
}

int main() {
    printf("Hello, world!\n");

    auto start = high_resolution_clock::now();

    for (int i = 0; i < 10000; ++i) {
        const Result<void> r = safe_main();
        assert_result(r);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    printf("took %d ms (%f ms)\n", duration.count(), (double)duration.count() / 10000.0);

    return 0;
}
