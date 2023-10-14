#include <unity.h>
#include "pid.hpp"
#include <ctime>
#include "log.hpp"

void test_init() {
    // Positive error -> negative steering angle
    PidControlClass pid(1.0, 1.0, 1.0, 0.0);
    clock_t start = clock();
    while (clock() - start < (CLOCKS_PER_SEC/1000) ) {}
    TEST_ASSERT(pid.getSteering(1.0, clock()) < 0.0); 
    // Negative error -> positive steering angle
    pid = PidControlClass(1.0, 1.0, 1.0, 0.0);
    start = clock();
    while (clock() - start < (CLOCKS_PER_SEC/1000) ) {}
    TEST_ASSERT(pid.getSteering(-1.0, clock()) > 0.0);
    // No error -> no steering angle
    pid = PidControlClass(1.0, 1.0, 1.0, 0.0);
    start = clock();
    while (clock() - start < (CLOCKS_PER_SEC/1000) ) {}
    TEST_ASSERT(abs(pid.getSteering(0.0, clock())) <  0.00001);
}

void test_with_strange_function() {
    // Save it to a file 
    // Create the file
    FILE *fp = fopen("test/.output/strange_func.csv", "w");
    if (fp == NULL) {
        LOG_ERROR("Test", "Failed to open file.");
        return;
    }
    // Write the header
    fprintf(fp, "time,sin,steering\n");
    PidControlClass pid(0.4, 0.2, 0.2, 0.0);
    for (int i = 0; i < 100; i++) {
        clock_t start = clock();
        while (clock() - start < (CLOCKS_PER_SEC/10000) ) {}

        double e = ((double)i/10)*exp(abs(cos((double)i/10)));
        double angle = pid.getSteering(e, clock());
        fprintf(fp, "%d,%f,%f\n", i, e, angle);
    }
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_init);
    #ifdef TEST_PID_MANUAL
        RUN_TEST(test_with_strange_function);
    #endif
    UNITY_END();
}
